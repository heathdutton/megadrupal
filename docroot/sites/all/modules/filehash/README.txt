FILE HASH
---------

Hashes of uploaded files, which can be found on a variety of sites from
archive.org to wikileaks.org, allow files to be uniquely identified, allow
duplicate files to be detected, and allow copies to be verified against the
original source.

File hash module generates and stores MD5, SHA-1 and/or SHA-256 hashes for each
file uploaded to the site.

Hash algorithms can be enabled and disabled by the site administrator.

Hash values are loaded into the $file object where they are available to the
theme and other modules.

Handlers are provided for Views module compatibility. In addition, a
<media:hash> element is added for file attachments in node RSS feeds (file,
image, and media field types are supported).

Tokens are provided for the full hashes, as well as pairtree tokens useful for
content addressable storage. For example, if the MD5 hash for a file is
3998b02c5cd2723153c39701683a503b, you could store it in the files/39/98
directory using these tokens: [filehash-md5-pair-1]/[filehash-md5-pair-2].
