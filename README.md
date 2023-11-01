# disaStr
_A Decentralized Information Sharing Architecture using nostr for gossip_

*Data Sources* can be plugged to access various data storage platforms (a data folder, a git repository,...).<br>
*nostr* is used to gossip who has what data, and who wants it.<br>
Data exchange is made using peer to peer connections. Users who have access to the repository can act as a *provider* and help broadcast the repository data.<br>

## Data providers
Any data storing system can be fit to be used for sharing. It must have the following properties:
- A data repository has a repository ID as a hash and a name
- A repository can optionnally have subfolders
- The repository can have versionning for its files, that will be publicly represented as hashes (like git commit hashs)

## Nostr Gossip:
Local user accounts are associated with a nostr identity for communication

Clients will send events for the following meaning:<br>
EV1 : I have repo *repo_hash*<br>
Clients will be able to request the follwing<br>
REQ1 : Who has the repo *repo_hash*?<br>

## Security :
P2P connection use encrypted socket connections

## Privacy
- The repository id is not publicly associated with a repository name. Only someone who has access credentials get's it's id
- REQ1 comes with a field containing an access key granted when access was given to a repository, so providers can ignore spam repository requests. And a range of data (subpath, specific version hash)
- EV1 comes with a hash of some specific data of the repo to prove ownership

## Permissions
The owner can give various access permissions to a user.<br>
Each granted permissions creates some data, and a hash (with the owner's private key) of that data is given to the user. The owner can then revoke those permissions.
Data access can ONLY be granted by the owner<br>
Permissions can be shared as read only data so other users who have the repository data can share it and help scale the bandwidth demand as the repository grows. _The owner must be aware that permission revocation will not be instantly braodcasted to everyone before allowing shared repository broadcasting_

## Onboarding
The process of giving access to a user is not part of the p2p exchanges. An API provides the functionality, and gives back som data to provide the user. This can be integrated any way you wish

## Data integrity
When acces is granted, the owner shares with a user a hash of a specific range of data (randomly chosen for that specific user) of the repository. This is in randomly generated 1MB chunk of data that all broadcasters will have to store permanently.<br>
A user can query broadcasters for the hash of their test range, and depending on the response, know if data was altered, or it they just pretend to have it.<br>
Any user,broadcaster,owner, will also regularly do requests for random ranges. Not only on the _bootstrap data_, also on the actual shared data that it already has in the form of pretend data requests to multiple users. If local data corruption is detected, data will be re-downloaded. If suspicious data is received on multiple occurences by a same broadcaster, that broadcaster will be blacklisted by that user (or it's access will be revoked if it's a test by the owner)<br>
When a user or the owner detects wrong returns, it signals the broadcaster of potential data loss, so he has an opportunity to update. After multiple errors detected by the oswner, the suspicious broadcaster might have access revocated.

## Data property
A owner should consider that any data it shared is now also the property of the user it shared read access with.<br>
Any user has a 'one-click' option to create a fork of a repository it has a copy of, with all the initial data except for owner-signed metadata (permissions,..), that will be reset<br>

## Implementations, current state & roadmap
The project is cutrrently into _thinking_ process. It will probably start with data providers with simple folder and/or git repo sharing data providers.<br>
First implementations might use a mix of python (for parts that do not need high performance and should be auditable) and C++ (for performance intensive functions)
