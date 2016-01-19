# Commerce Credits Transaction

Commerce Credits Transaction extends the Commerce Credits controller class, and
implements a transaction entity to allow credit transfers to be reversible.

The primary use case is for a user to pledge credits to another entity.

## Changes to Commerce Credits Behavior

- Commerce Credits controller class is overridden by Commerce Credits Transaction:
   - All Commerce Credits operations will create a transaction.
   - Remove credits will *not* delete the credits entity if the amount of credits is less than 1.
- Rules
   - It is recommended to use the Commerce Credits Transaction transfer rule action instead of the Commerce Credits transfer rule action.
- Widget
   - An improved widget that uses CTools should be used for paying or pledging credits.
   - A CTools content-type plugin is provided that requires a node context.

## Usage

- Create a rule to add credits on checkout for commerce credits product per Commerce Credits instructions.
- Add a link to the pay widget for an entity with the `ctools-use-modal` class:
   - `credits/transaction/{CREDITTYPE}/{ENTITYTYPE}/{ENTITYID}/nojs`
     - CREDITTYPE refers to the Commerce Credits group type. By default, this is "credit" if Commerce Credits UI was installed.
     - ENTITYTYPE refers to the Drupal entity type. In this case "node".
     - ENTITYID refers to the Drupal entity id. In this case the "Node ID" or "nid".
     - Optionally the entity to transfer from can be added to the end of the URL if the user has access to do this. By default, the current user will pay from their credits store.
   - Example: `<a href="credits/transaction/credit/node/1/nojs" class="ctools-use-modal">Pay</a>`
   - Or use the CTools content-type plugin to add the widget into Panels for nodes.
- Create a rule to reverse credits from an entity such as when a node is unpublished or a field on a node changes to a different value.
- Review transactions at `admin/commerce/credits/transaction`.
