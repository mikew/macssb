import Cocoa

class WebView: WKWebView {
    // Disable beep.
    override func keyDown(with event: NSEvent) {
        // This seems a little weird, but there's caveats to using only one of `keyDown` or `performKeyEquivalent` to stop beeps:
        // Overriding `keyDown` stops key presses from getting passed down.
        // Overriding `performKeyEquivalent` prevents menu shortcuts from working.
        self.performKeyEquivalent(with: event)
    }
}
