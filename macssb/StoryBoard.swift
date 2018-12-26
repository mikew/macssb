import Foundation

class StoryBoard {
    static func instantiateController(identifier: String, storyboard: String?) -> Any {
        return NSStoryboard.init(name: storyboard ?? "Main", bundle: nil)
            .instantiateController(withIdentifier: identifier)
    }
}
