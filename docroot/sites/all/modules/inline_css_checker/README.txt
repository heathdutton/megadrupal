Inline CSS Checker
==================

This module is intended to be a useful hack. It iterates over every node in the
system and renders it. It then loads the result into a DOMDocument and retrieves
the inner HTML for it. Nodes don't contain <head> information, so this resists
false positives.

It uses XPath selection to check if <style> tags or style attributes on tags
are in the text.

It puts some information about its findings and the discovered snippets into a
table and shows them under the button.
