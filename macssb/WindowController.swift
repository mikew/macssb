import Cocoa

class WindowController: NSWindowController {
    static var allWindowHistory = [[String]]()
    static var currentWindowHistory = [String]()
    static var allWindows = [WindowController]()
    var w = TWCDebouncer()

    override func windowDidLoad() {
        super.windowDidLoad()
        // Hide tab bar by default.
        // If more than one tab exist the bar is always shown.
        self.window?.toggleToolbarShown(nil)

        if UserDefaults.standard.string(forKey: "AppleInterfaceStyle") == "Dark" {
            self.window?.appearance = NSAppearance.init(named: .vibrantDark)
        }
    }
    
    override func keyUp(with event: NSEvent) {
        print(event)
    }
}

extension WindowController: NSWindowDelegate {
    func window(_ window: NSWindow, willUseFullScreenPresentationOptions proposedOptions: NSApplication.PresentationOptions = []) -> NSApplication.PresentationOptions {
        return [
            .autoHideToolbar,
            proposedOptions
        ]
    }

    func windowShouldClose(_ sender: NSWindow) -> Bool {
        self.w.handleClose(window: self.window!)
        (self.contentViewController as! WebViewController).webView._close()
        return true
    }
}

// Stands for TabbedWindowCloseDebouncer
class TWCDebouncer {
    private static var Debouncers = [NSWindowTabGroup: () -> Void]()
    private static var TabStartCounts = [NSWindowTabGroup: Int]()
    private static var Windows = [NSWindowTabGroup: [NSWindow]]()
    private static var LastTabGroup: NSWindowTabGroup?
    public static var OnCloseSingle = Event<NSWindow>()
    public static var OnCloseMany = Event<[NSWindow]>()
    public static var OnCloseAll = Event<[NSWindow]>()

    func handleClose(window: NSWindow) {
        // This handles a window without tabs.
        // Check presence in Debouncers, otherwise it will also catch the last
        // window of a tabbed window closing.
        if window.tabGroup == nil {
            TWCDebouncer.OnCloseAll.raise(data: [window])
            return
        }
        if window.tabbedWindows == nil && TWCDebouncer.Debouncers[window.tabGroup!] == nil {
            TWCDebouncer.OnCloseAll.raise(data: [window])
            return
        }

        // This could probably lead to problems.
        TWCDebouncer.LastTabGroup = window.tabGroup

        // Store the initial tab count.
        if TWCDebouncer.TabStartCounts[TWCDebouncer.LastTabGroup!] == nil {
            TWCDebouncer.TabStartCounts[TWCDebouncer.LastTabGroup!] = window.tabbedWindows?.count
        }

        // Initialize the list of windows closing.
        if TWCDebouncer.Windows[TWCDebouncer.LastTabGroup!] == nil {
            TWCDebouncer.Windows[TWCDebouncer.LastTabGroup!] = []
        }

        // Set up the debounced function.
        if TWCDebouncer.Debouncers[TWCDebouncer.LastTabGroup!] == nil {
            TWCDebouncer.Debouncers[TWCDebouncer.LastTabGroup!] = debounce(delay: .milliseconds(20), action: {
                let countAfter = TWCDebouncer.LastTabGroup?.windows.count ?? 0

                if countAfter == 0 {
                    // All windows were closed.
                    TWCDebouncer.OnCloseAll.raise(data: TWCDebouncer.Windows[TWCDebouncer.LastTabGroup!]!)
                } else {
                    // One or more windows were closed in the tab group
                    TWCDebouncer.OnCloseMany.raise(data: TWCDebouncer.Windows[TWCDebouncer.LastTabGroup!]!)
                }

                // Reset.
                TWCDebouncer.Debouncers[TWCDebouncer.LastTabGroup!] = nil
                TWCDebouncer.TabStartCounts[TWCDebouncer.LastTabGroup!] = nil
                TWCDebouncer.Windows[TWCDebouncer.LastTabGroup!] = nil
                TWCDebouncer.LastTabGroup = nil
            })
        }

        // Store the window.
        TWCDebouncer.Windows[TWCDebouncer.LastTabGroup!]?.append(window)
        // Call the debounced function.
        TWCDebouncer.Debouncers[window.tabGroup!]!()
    }
}
