import Cocoa
import WebKit

class WebViewController: NSViewController {
    static let ProcessPool: WKProcessPool = WKProcessPool.init()

    var webView: WKWebView!
    var initialUrl: String! = AppDelegate.Config.homepage

    var currentUrl: String {
        if self.webView != nil {
            return (self.webView.url?.absoluteString)!
        }

        return self.initialUrl
    }
    
    override func viewDidLoad() {
        super.viewDidLoad()
        
        self.webView = self.createWebView()
        
        self.webView.translatesAutoresizingMaskIntoConstraints = false

        // Adding as a subview instead of the WebView being the view fixes a
        // crash when videos are leaving full screen.
        self.view.addSubview(self.webView)
        self.webView.topAnchor.constraint(equalTo: self.view.topAnchor).isActive = true
        self.webView.leadingAnchor.constraint(equalTo: self.view.leadingAnchor).isActive = true
        self.webView.bottomAnchor.constraint(equalTo: self.view.bottomAnchor).isActive = true
        self.webView.trailingAnchor.constraint(equalTo: self.view.trailingAnchor).isActive = true

        let blockListAsset = NSDataAsset.init(name: "blockerList")
        let blockList = String(data: blockListAsset!.data, encoding: String.Encoding.utf8)!

        WKContentRuleListStore.default().compileContentRuleList(forIdentifier: "ContentBlockingRules", encodedContentRuleList: blockList) { (ruleList, error) in
            if error != nil {
                print(error as Any)
                
                return
            }
            
            self.webView.configuration.userContentController.add(ruleList!)
            if AppDelegate.IsFirstLaunch {
                self.loadInitialUrl()
            }
        }
        
        self.becomeFirstResponder()
    }

    func createWebView() -> WKWebView {
        let configuration = WKWebViewConfiguration()

        // This shares cookies across tabs.
        configuration.processPool = WebViewController.ProcessPool

        configuration.allowsAirPlayForMediaPlayback = true
        configuration.mediaTypesRequiringUserActionForPlayback = .all

        configuration.preferences.plugInsEnabled = true
        configuration.preferences._fullScreenEnabled = true
        configuration.preferences._developerExtrasEnabled = true
        configuration.preferences._mediaDevicesEnabled = true
        configuration.preferences._isStandalone = true
        configuration.preferences._allowsPictureInPictureMediaPlayback = true
        configuration.preferences.javaScriptCanOpenWindowsAutomatically = true
        configuration.preferences.tabFocusesLinks = true

        // TODO Setting this to false seems to make mapbox-gl perform better?
        configuration.preferences._acceleratedDrawingEnabled = false

        let webView = WebView(frame: .zero, configuration: configuration)
        webView.allowsBackForwardNavigationGestures = true
        webView.allowsLinkPreview = true
        webView.allowsMagnification = true
        webView.customUserAgent = "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_13_2) AppleWebKit/604.4.7 (KHTML, like Gecko) Version/11.0.2 Safari/604.4.7"

        webView.navigationDelegate = self
        webView.uiDelegate = self
        webView.addObserver(self, forKeyPath: "title", options: .new, context: nil)
        webView.addObserver(self, forKeyPath: "estimatedProgress", options: .new, context: nil)

        return webView
    }

    func loadInitialUrl() {
        if initialUrl != nil {
            self.visitUrl(url: initialUrl)
        }
    }
    
    func visitUrl(url: String) {
        self.webView.load(URLRequest.init(url: URL.init(string: url)!))
    }

    override func observeValue(forKeyPath keyPath: String?, of object: Any?, change: [NSKeyValueChangeKey : Any]?, context: UnsafeMutableRawPointer?) {
        if keyPath == "title" {
            self.view.window?.title = self.webView.title!
        }

        if keyPath == "estimatedProgress" {
            if self.webView.estimatedProgress == 1.0 {
                self.view.window?.title = self.webView.title!
            } else {
                self.view.window?.title = "⌛ \(self.webView.title!)"
            }
        }
    }
    
    @objc func handleBookmark(_ sender: Any) {
        if let menuItem = sender as? BookmarkMenuItem {
            self.visitUrl(url: menuItem.URL)
        }
    }

    @IBAction func videoPIP(_ sender: Any?) {
        self.webView.evaluateJavaScript("document.querySelector('video').webkitSetPresentationMode('picture-in-picture')", completionHandler: nil)
    }
    
    @IBAction func videoAirPlay(_ sender: Any?) {
        self.webView.evaluateJavaScript("document.querySelector('video').webkitShowPlaybackTargetPicker()", completionHandler: nil)
    }

    @IBAction func openDocument(_ sender: Any?) {
        let panel: NSAlert = NSAlert()
        panel.messageText = "Open Location"
        panel.alertStyle = .informational
        panel.addButton(withTitle: "OK")
        panel.addButton(withTitle: "Cancel")

        let input = NSTextField(frame: NSRect(x: 0, y: 0, width: (self.view.window?.frame.width)! - 256, height: 24))
        input.stringValue = (self.webView.url?.absoluteString)!
        panel.window.initialFirstResponder = input
        panel.accessoryView = input

        panel.beginSheetModal(for: self.view.window!, completionHandler: { (res) in
            if res == NSApplication.ModalResponse.alertFirstButtonReturn {
                self.visitUrl(url: input.stringValue)
            }
        })
    }

    @IBAction func reloadPage(_ sender: Any?) {
        self.webView.reload(sender)
    }

    deinit {
        self.webView._close()
    }
}

extension WebViewController: WKNavigationDelegate {
    func webView(_ webView: WKWebView, decidePolicyFor navigationAction: WKNavigationAction, decisionHandler: @escaping (WKNavigationActionPolicy) -> Void) {
        // TODO handle links to outside world.

        if navigationAction.navigationType == WKNavigationType.linkActivated {
            // Handle `<a target="_blank" />`.
            if navigationAction.targetFrame == nil
                // Handle CMD+Click
                || navigationAction.modifierFlags.contains(NSEvent.ModifierFlags.command)
                // Handle middle mouse button. Seems strange it's 4 but not 3.
                || navigationAction.buttonNumber == 4
            {
                (NSApp.delegate as! AppDelegate).createWindow(
                    shouldCreateTab: true,
                    shouldOrderFront: false,
                    url: navigationAction.request.url?.absoluteString
                )
                decisionHandler(.cancel)

                return
            }
        }

        decisionHandler(.allow)
    }
}

extension WebViewController: WKUIDelegate {
    // Handle `window.open` / "Open link in new window".
    func webView(_ webView: WKWebView, createWebViewWith configuration: WKWebViewConfiguration, for navigationAction: WKNavigationAction, windowFeatures: WKWindowFeatures) -> WKWebView? {
        (NSApp.delegate as! AppDelegate).createWindow(
            shouldCreateTab: true,
            shouldOrderFront: false,
            url: navigationAction.request.url?.absoluteString
        )

        return nil
    }

    func webView(_ webView: WKWebView, runOpenPanelWith parameters: WKOpenPanelParameters, initiatedByFrame frame: WKFrameInfo, completionHandler: @escaping ([URL]?) -> Void) {
        let openDialog = NSOpenPanel()
        openDialog.allowsMultipleSelection = parameters.allowsMultipleSelection

        openDialog.beginSheetModal(for: self.view.window!, completionHandler: { (res) -> Void in
            if res == NSApplication.ModalResponse.OK {
                completionHandler(openDialog.urls)
            } else {
                completionHandler(nil)
            }
        })
    }

    func webView(_ webView: WKWebView, runJavaScriptAlertPanelWithMessage message: String, initiatedByFrame frame: WKFrameInfo, completionHandler: @escaping () -> Void) {
        let alert: NSAlert = NSAlert()
        alert.messageText = "Alert"
        alert.informativeText = message
        alert.alertStyle = .warning
        alert.addButton(withTitle: "OK")

        alert.beginSheetModal(for: self.view.window!, completionHandler: { (res) in
            completionHandler()
        })
    }

    func webView(_ webView: WKWebView, runJavaScriptConfirmPanelWithMessage message: String, initiatedByFrame frame: WKFrameInfo, completionHandler: @escaping (Bool) -> Void) {
        let alert: NSAlert = NSAlert()
        alert.messageText = "Confirm"
        alert.informativeText = message
        alert.alertStyle = .warning
        alert.addButton(withTitle: "OK")
        alert.addButton(withTitle: "Cancel")

        alert.beginSheetModal(for: self.view.window!, completionHandler: { (res) in
            if res == NSApplication.ModalResponse.alertFirstButtonReturn {
                completionHandler(true)
            } else {
                completionHandler(false)
            }
        })
    }

    func webView(_ webView: WKWebView, runJavaScriptTextInputPanelWithPrompt prompt: String, defaultText: String?, initiatedByFrame frame: WKFrameInfo, completionHandler: @escaping (String?) -> Void) {
        let alert: NSAlert = NSAlert()
        alert.messageText = "Prompt"
        alert.informativeText = prompt
        alert.alertStyle = .warning
        alert.addButton(withTitle: "OK")
        alert.addButton(withTitle: "Cancel")

        let input = NSTextField(frame: NSRect(x: 0, y: 0, width: 300, height: 24))
        input.stringValue = defaultText!
        alert.window.initialFirstResponder = input
        alert.accessoryView = input

        alert.beginSheetModal(for: self.view.window!, completionHandler: { (res) in
            if res == NSApplication.ModalResponse.alertFirstButtonReturn {
                completionHandler(input.stringValue)
            } else {
                completionHandler(nil)
            }
        })
    }
}

extension WebViewController: _WKDownloadDelegate {
    // TODO This doesn't seem to get called, even though the other is deprecated.
    func _download(_ download: _WKDownload!, decideDestinationWithSuggestedFilename filename: String!, completionHandler: ((Bool, String?) -> Void)!) {
        print(download)
        let saveDialog = NSSavePanel()
        saveDialog.canCreateDirectories = true
        saveDialog.nameFieldStringValue = filename

        saveDialog.beginSheetModal(for: self.view.window!, completionHandler: { (res) in
            if res == NSApplication.ModalResponse.OK {
                completionHandler(true, saveDialog.url?.absoluteString)
            } else {
                download.cancel()
                completionHandler(false, nil)
            }
        })
    }

    // TODO This shos a dialog sometimes, but never does any actual saving.
    func _download(_ download: _WKDownload!, decideDestinationWithSuggestedFilename filename: String!, allowOverwrite: UnsafeMutablePointer<ObjCBool>!) -> String! {
        let saveDialog = NSSavePanel()
        saveDialog.canCreateDirectories = true
        saveDialog.nameFieldStringValue = filename

        saveDialog.beginSheetModal(for: self.view.window!, completionHandler: { (res) in
            NSApp.stopModal(withCode: res)
        })
        if NSApp.runModal(for: self.view.window!) == NSApplication.ModalResponse.OK {
            return saveDialog.url?.absoluteString
        }

        download.cancel()

        return ""
    }
}

