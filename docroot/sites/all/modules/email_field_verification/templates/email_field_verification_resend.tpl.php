<?php

/**
 * @file
 * My God! pareview.sh is really picky.
 */

?>
<div class="form-item">
<?php
  print l(
    t('Resend verification link', array(), array('context' => 'email_field_verification')),
    'email/verification/resend/nojs/' . $entity_type . '/' . $entity_id . '/' . $field_name . '/' . $delta . '/' . $email,
    array(
      'attributes' => array('class' => array('use-ajax'), 'target' => '_blank'),
      'query' => array('token' => $token),
    )
  );
?>
  <div id="email_field_verficication_message_<?php print $field_name . '_' . $delta; ?>" />
</div>
