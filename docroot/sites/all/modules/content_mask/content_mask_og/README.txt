Introduction
------------
Extends the Content Mask module with Organic Group support.
You can now mask certain parts of your content, based on the user's
Organic Group memberships, e.g.
```
[content_mask og_type="node" og_group="1"]
Praesent rutrum, ligula vitae porta faucibus, nunc ante consequat est, in
imperdiet eros odio vitae nunc.
[/content_mask]

[content_mask og_type="node" og_role="administrator|member"]
Mauris ornare augue ac augue tempor auctor. Praesent justo ligula, convallis
quis semper a, eleifend ut mi. Etiam eleifend aliquet quam, accumsan eleifend
erat vestibulum at. Pellentesque habitant morbi tristique senectus et netus et
malesuada fames ac turpis egestas.
[/content_mask]
```

Requirements
------------
- content_mask
- og
