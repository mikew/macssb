class HostBlockList {
    var BlockedHosts = [String: Bool]()

    init() {
    }

    func isHostBlocked(host: String?) -> Bool {
        if host == nil {
            return false
        }

        return BlockedHosts[host!] != nil
    }

    func loadFromAssetFile() {
        let t = NSDataAsset.init(name: "hosts")
        self.addFromString(hosts: String(data: t!.data, encoding: String.Encoding.utf8)!)
    }

    func addFromString(hosts: String) {
        let skip = [
            "localhost",
            "localhost.localdomain",
            "local",
            "broadcasthost",
            "localhost",
            "localhost",
            "ip6-allnodes",
            "ip6-allrouters",
            "0.0.0.0",
        ]

        hosts.enumerateLines { (line, _) in
            if line == "" || line.hasPrefix("#") {
                return
            }

            let parts = line.components(separatedBy: " ")

            if skip.contains(parts[1]) {
                return
            }

            self.BlockedHosts[parts[1]] = true
        }

    }
}
