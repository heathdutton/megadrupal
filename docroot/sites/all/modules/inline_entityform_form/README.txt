Provides a way to expose an entityform in-line via an entityreference field.

Install and enable the module, then add an entityreference field with 'Entityform
Submission' as target entity type and the 'Inline entity form' widget.

On the 'Manage didplay' page, choose to show the field asa "Rendered entity' and
configure the view mode you want.

== LIMITATIONS ==

The module currently saves all form submissions in Draft mode, the idea is to use
Rules to save a submission permanently when a Commerce order clears checkout. If 
that doesn't work at all for your use case, simply comment out 'draft = 1' in the
include file.
