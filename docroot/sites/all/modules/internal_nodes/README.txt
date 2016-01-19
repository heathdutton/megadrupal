INTERNAL NODES
--------------
By eosrei and mjpa

DESCRIPTION
-----------
Some content/nodes should never be viewed directly; only visible through
something else such as Views, or Panels. Per-content type or per-node, this
module denies access to node/[nid] URLs while allowing the content to stay
published and otherwise viewable.

FUNCTIONALITY
-------------
 * Creates a permission per content type which when disabled denies node view
   to the specified role.
 * Adds an option to content type edit forms for administrators to select the
   result of a direct node view.
 * Node view options are Allow, Not Found, Access Denied, and Redirect.
 * Redirect URL/path can use tokens, an anchors or get variables.
 * "Denied action setting per-node" in content type settings. Node default is
   to use content type default.
 * hook_url_outbound_alter() implementation to rewrite URLs of denied nodes
   to the redirect URL.
 * Simpletests for content type settings, node settings, and
   hook_url_outbound_alter().
 * Rules integration - Node view denied to user
 * intnode-denied class on <div> and optional status messages for users with
   permission to view internal nodes.

USE EXAMPLES
------------
 * Index all content types for search, but redirect some to correct to the
   View/Panel when accessed.
 * A View/Panel can open/scroll/page to a node when a [node:nid] token is used
   as a GET variable and or Anchor.
 * Deny access to node/[nid] URLs, except where required.


HISTORY
-------
4/23/11 - Internal nodes created by mjpa, and submitted as a Project App.
4/27/11 - Block direct view created by eosrei, and submitted as a Project App.
8/08/11 - Block direct view renamed/replaced Internal nodes and promoted.
