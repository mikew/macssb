class ConfigReader {
    var homepage: String!
    var bookmarks: [[String]] = []

    init() {
        self.loadFromAssetFile()
    }

    func loadFromAssetFile() {
        let t = NSDataAsset.init(name: "config")
        self.readFromData(data: t!.data)
    }

    func readFromData(data: Data) {
        let json = try? JSONSerialization.jsonObject(with: data, options: []) as! [String: Any]
        
        if let homepage = json!["homepage"] as? String {
            self.homepage = homepage
        }
        
        if let bookmarks = json!["bookmarks"] as? [[String]] {
            self.bookmarks = bookmarks
        }
    }
}
