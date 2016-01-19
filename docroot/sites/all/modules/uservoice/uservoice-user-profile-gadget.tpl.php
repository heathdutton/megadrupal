<?php
/**
 * @file
 * Template file for user profile gadget. This gadget is designed to be
 * displayed user profile information from your Drupal site within the UserVoice
 * admin console (fetched by UserVoice via ajax callback).
 *
 * Available variables:
 * - $gadget_title: The gadget title.
 * - $account: User account object associated with this gadget request.
 * - $user_profile_fields: An array of renderable user profile fields associated
 *   with the requested account.
 *
 * @see uservoice_user_profile_gadget()
 * @see template_preprocess_uservoice_user_profile_gadget()
 * @see https://developer.uservoice.com/docs/gadgets/creating-your-own/
 */
?>
<?php global $base_url; ?>
<!DOCTYPE html>
<html>
  <head>
    <title><?php print $gadget_title; ?></title>
    <link href="https://cdn.uservoice.com/packages/gadget.css" media="all" rel="stylesheet" type="text/css" />
    <link href="<?php print $base_url . '/' . drupal_get_path('module', 'uservoice'); ?>/uservoice.gadget.css" media="all" rel="stylesheet" type="text/css" />
  </head>
  <body>

    <!-- Check that we have something to show in the gadget. -->
    <?php if (!empty($account)): ?>
      <table class="fields">
        <thead>
          <caption><?php print $gadget_title; ?></caption>
        </thead>
        <tbody>

          <!-- Picture. -->
          <?php $user_picture = theme('user_picture', array('account' => $account)); ?>
          <?php if (!empty($user_picture)): ?>
            <tr>
              <th title="Picture">Picture</th>
              <td><?php print $user_picture; ?></td>
            </tr>
          <?php endif; ?>

          <!-- User profile fields. -->
          <?php foreach ($user_profile_fields as $field): ?>
            <tr>
              <th title="<?php print $field['#title']; ?>"><div><?php print $field['#title'];?></div></th>
              <td><?php print render($field); ?></td>
            </tr>
          <?php endforeach; ?>

          <!-- Roles. -->
          <tr>
            <th title="Roles">Roles</th>
            <td><?php print theme('item_list', array('items' => $account->roles)); ?></td>
          </tr>
        </tbody>
      </table>

    <!-- Nothing to show: let UserVoice know that we have an empty gadget. -->
    <?php else: ?>
      <script type="text/javascript">
        window.gadgetNoData = true;
      </script>
    <?php endif; ?>
    <script src="https://cdn.uservoice.com/packages/gadget.js" type="text/javascript"></script>
  </body>
</html>
