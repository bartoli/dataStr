# dataStr
A data sharing platform on nostr

The sharer registers a repository of data, with optional subdirectories and versions support

The repository can be public or private

When private, the sharer creates keys that allow read and/or write access to parts of the repository.

Keys are sent by pair.
- K1: One key allows to show that you have access authorization. It is used to decrypt the data paths in the data requests
- K2: One allows to decrypt the data received by the repo

User EVENT0 : i have a repository
params : repo id (hash)

User request EVENT1 : Who has a copy of this repository?
Params:
 - repo id (hash) given by repo owner
 - contact WSS address
Answer OK1 :
 - relays that have a c
