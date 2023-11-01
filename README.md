# disaStr
A Decentralized Information Sharing Architecture using nostr for gossip

*Data Sources* can be plugged to access various data storage platforms (a data foldder, a git repository,...).<br>
*nostr* is used to gossip who has what data, and who wants it.<br>
Users who have aces to the repository can help broadcast the repository data. Data excahange is made using peer to peer connections.<br>


## Data providers
Any data storing system can be fit to be used for sharing. It must have the following properties:
- A data repository has a repository ID as a hash and a name
- A repository can optionnally have subfolders
- The repository can have versionning for its files, that will be publicly represented as hashes (like git commit hashs)

## Nostr Gossip:
Local user accounts are associated with a nostr identity for comunication

Clients will send events fo the following meaning
EV1 : I have repo *repo_hash*
Clients will be able to request the follwing
REQ1 : Who has the repo *repo_hash*?


## Security :
P2P connection usr secure socket connections

## Privacy
- The repository id is not publicly associated with a repository name. Only someone who has access credentials get's it's id
- REQ1 comes with a field containing an access key granted when access was given to a repository, so providers can ignore spam repository requests
- EV1 comes wiht a hash of some data of the repo 




P2P events:
The sharer registers a repository of data, with optional subdirectories and versions support

The repository can be public or private

When private, the sharer creates keys that allow read and/or write access to parts of the repository.

Keys are sent by pair.
- K1: One key allows to show that you have access authorization. It is used to decrypt the data paths in the data requests
- K2: One allows to decrypt the data received by the repo
