The simplest and easiest way to add a social buttons (Facebook Like, Google+, Twitter share ...) to your contents.
Social Buttons is a field and using the field API, which make it attachable to any entity, exposed to Views, exportable through Features, and so on...
It is possible to include configuration parameters into the social buttons code. Just use tokens for this.

Here is an example:

<div class="fb-like" data-layout="button_count" data-send="false" data-show-faces="false"
data-href="[node:url:absolute]" data-width="1"></div>

To have additional token support, for example some missing core tokens or token browser,
just install Tokens module