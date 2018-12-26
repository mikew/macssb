import Cocoa

@NSApplicationMain
class AppDelegate: NSObject, NSApplicationDelegate {
    static let Config = ConfigReader()
    static var IsFirstLaunch = true
    var closeAllHandler: Disposable?
    var closeManyHandler: Disposable?

    func applicationDidFinishLaunching(_ aNotification: Notification) {
        AppDelegate.IsFirstLaunch = false
        self.closeAllHandler = TWCDebouncer.OnCloseAll.addHandler(target: self, handler: AppDelegate.handleCloseAll)
        self.closeManyHandler = TWCDebouncer.OnCloseMany.addHandler(target: self, handler: AppDelegate.handleCloseMany)
        
        if (AppDelegate.Config.bookmarks.count > 0) {
            let menu = NSApp.mainMenu
            let historyMenu = menu?.item(withTitle: "History")
            
            for (i, bookmarkSpec) in AppDelegate.Config.bookmarks.enumerated() {
                let name = bookmarkSpec[0]
                let url = bookmarkSpec[1]
                let menuItem = BookmarkMenuItem.init(title: name, action: #selector(WebViewController.handleBookmark(_:)), keyEquivalent: String(i + 1))
                menuItem.keyEquivalentModifierMask = [.command, .option]
                menuItem.URL = url
                historyMenu?.submenu?.addItem(menuItem)
            }
        }
    }

    func handleCloseAll(data: [NSWindow]) {
        print("handleCloseAll")
        print(data)
    }

    func handleCloseMany(data: [NSWindow]) {
        print("handleCloseMany")
        print(data)
    }

    func applicationWillTerminate(_ aNotification: Notification) {
        closeAllHandler?.dispose()
    }

    // "New Window" in the main menu.
    @IBAction func newDocument(_ sender: Any?) {
        self.createWindow(shouldCreateTab: false, shouldOrderFront: true, url: nil)
    }

    // This gets you the "new tab" button in the tab bar.
    @IBAction func newWindowForTab(_ sender: Any?) {
        self.createWindow(shouldCreateTab: true, shouldOrderFront: true, url: nil)
    }

    func createWindow(shouldCreateTab: Bool, shouldOrderFront: Bool, url: String?) {
        let controller = StoryBoard.instantiateController(identifier: "WindowController", storyboard: nil) as! WindowController
        // Without this random things get broken like the tab bar not hiding
        // in full screen when not on the first tab.
        // Too bad removing items from it causes an unrelated crash.
        WindowController.allWindows.append(controller)

        if shouldCreateTab {
            // This is brittle due to using `keyWindow`. It's tough to tell
            // whether this line should live in AppDelegate, or
            // WindowController.
            NSApp.keyWindow?.addTabbedWindow(controller.window!, ordered: .above)
        } else {
            // This doesn't make a whole lot of sense:
            // Set the tabbingMode to something other than "preferred". Even
            // something like "disallowed" works, and doesn't actually stop us
            // from creating new tabs in the new window.
            // Q. So why not just set the default tabbingMode to automatic in
            // the storyboard?
            // A. Because then something like "show all tabs" breaks, even
            // though changing the same setting to the same value here have
            // that same effect.
            controller.window!.tabbingMode = .automatic
            // controller.window!.moveTabToNewWindow(nil)
        }

        if shouldOrderFront {
            controller.window?.makeKeyAndOrderFront(self)
        }

        let contentViewController = controller.contentViewController as! WebViewController
        contentViewController.initialUrl = url ?? AppDelegate.Config.homepage
        contentViewController.loadInitialUrl()
    }

//    @IBAction func undoCloseTab(_ sender: Any?) {
//        let urls = WindowController.windowHistory.popLast()
//
//        print(urls)
//        if urls == nil || urls?.count == 0 {
//            // TODO This just launches a new window with the default homepage,
//            // would be nice if it was properly the last tab of the last
//            // closed window.
//            self.newDocument(sender)
//
//            return
//        }
//
//        WindowController.shouldShowInitialTabNext = false
//        let controller = StoryBoard.instantiateController(identifier: "WindowController", storyboard: nil) as! WindowController
//        controller.shouldCascadeWindows = true
//        controller.showWindow(sender)
////
////        for x in urls! {
////            _ = controller.tabViewController.addViewWithLabel(url: x)
////        }
//    }
}
