Introduction
------------
Mask parts of your content for certain users based on their role or UID.
The module adds the <em>content_mask</em> shortcode for defining parts
that should be masked, e.g.
```
Lorem ipsum dolor sit amet, consectetur adipiscing elit. Nulla ac ligula non
augue scelerisque dapibus vel et ante. Pellentesque volutpat nibh ut ipsum
bibendum euismod. Ut nibh nisi, aliquam sed ornare eu, hendrerit quis nunc. Nam
vel nunc sed erat dapibus accumsan id eget nunc.

[content_mask uid="1"]
Mauris ornare augue ac augue tempor auctor. Praesent justo ligula, convallis
quis semper a, eleifend ut mi. Etiam eleifend aliquet quam, accumsan eleifend
erat vestibulum at. Pellentesque habitant morbi tristique senectus et netus et
malesuada fames ac turpis egestas.
[/content_mask]

[content_mask role="editor|administrator"]
Praesent rutrum, ligula vitae porta faucibus, nunc ante consequat est, in
imperdiet eros odio vitae nunc.
[/content_mask]
```

The module uses Wordpress's shortcode regex as base for handling the masked
content.

Requirements
------------
- filter

Installation
------------
- Enable the module.
- Go to admin > configuration > content authoring > text formats.
- Click on a text format you would like to have content_mask support.
- Enable the content mask filter by clicking the checkbox.
- Add the content_mask shortcode on any content with the configured text format.
