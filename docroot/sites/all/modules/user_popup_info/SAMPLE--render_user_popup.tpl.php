<?php
/**
 * @file
 * User mini profile popup's sample theme implementation.
 *
 * Available variables:
 *
 *   $data (array): Array of popup specific variables that contains 
 *    information about mouse hovered user, following are the keys -
 * 
 *     - $data['name']: Name of the user, if realname is used, this will contain
 *                      realname of the user.
 *     - $data['picture']: Profile picture of user, or a default pic.
 *     - $data['pic_width']: Width of profile pic as set in the configuration page.
 *     - $data['pic_height']: Height of profile pic as set in the configuration page.
 *     - $data['member_since']: Contains how old this user is on your site.
 *     - $data['status']: Is the user active or blocked.
 *     - $data['cuser']: $user array.
 *     - $data['fields'] (array): Contains a list of fields found on user account page.
 * 
 */
?>

<?php
  $user_details = $variables['data'];
  if (($user_details['status'] == 'Active')) {
    ?>
    <div class="user-popup-processed-info">
    <?php
  }
  else {
    ?>
    <div style="border-left:5px solid #ed541d;background-color:#fef5f1;" class="user-popup-processed-info">
    <?php
  }
  ?>

  <div id="user-popup-display-details-pic">
  <span id="user-popup-pic"><img width="<?php echo $user_details['pic_width']; ?>" height="<?php echo $user_details['pic_height']; ?>" src="<?php echo $user_details['picture']; ?>" /></span>
  </div>

    <div id="user-popup-display-details">
      <span id="user-popup-name"><strong><?php echo $user_details['name']; ?></strong></span>

      <?php
      if (variable_get('user_popup_info_show_default_user_info', 'yes') == 'yes') {
      ?>
        <br>
        <span id="user-popup-member-since">Member since - <?php echo $user_details['member_since']; ?></span>
        <br>
        <span id="user-popup-status">Status - <?php echo $user_details['status']; ?></span>

      <?php
      }
      if (count($user_details['fields']) > 0) {
        $i = 0;
        foreach ($user_details['fields'] as $fname => $fval) {
        ?> 

          <br>
          <span class="user-popup-user-fields" id="user-popup-user-field-<?php echo ++$i; ?>"><?php echo $fval['label'] . ' - ' . $fval['value']; ?></span>
      <?php
        }
      }
      ?>
    </div>
  </div>
</div>