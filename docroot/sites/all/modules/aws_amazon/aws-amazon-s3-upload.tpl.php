<?php
global $base_url;
global $user;

$upload_key = 'direct-uploads/user/' . $user->uid . "/";
$s3url = 'https://' . $bucket . '.s3.amazonaws.com/';
$s3keys = array('s' => variable_get('aws_amazon_secret_key', ''), 'a' => variable_get('aws_amazon_access_key', ''));
$return_url = url('aws-amazon/upload/' . $bucket, array('absolute' => TRUE));

$policy_doc = '{"expiration": "2199-01-01T00:00:00Z",' . "\n" .
'"conditions": [' . "\n" .
'{"bucket": "' . $bucket . '"},' . "\n" .
'["starts-with", "$key", "' . $upload_key . '"],' . "\n" .
'{"acl": "private"},' . "\n" .
'{"success_action_redirect": "' . $return_url . '"},' . "\n" .
']' . "\n" .
'}';
$policy = base64_encode($policy_doc);
$signature= base64_encode(hash_hmac('sha1', $policy, $s3keys['s'], true));
?>

<form id="aws_upload_form" action="<?php print $s3url; ?>" method="post" enctype="multipart/form-data">
  <div class="form-item">
<input type="hidden" name="key" value="<?php print $upload_key; ?>${filename}">
<input type="hidden" name="AWSAccessKeyId" value="<?php print $s3keys['a'];?>">
<input type="hidden" name="acl" value="private">
<input type="hidden" name="success_action_redirect" value="<?php print $return_url; ?>">
<input type="hidden" name="policy" value="<?php print $policy; ?>">
<input type="hidden" name="signature" value="<?php print $signature; ?>">
<label for="file"><?php print t('Select File');?></label>
<input name="file" id="file_to_upload" type="file">
<div style="margin-top:20px;">
<input type="submit" class="form-submit" id="btn_upload" value="<?php print t('Upload');?>">
</div>
</div>
</form>
