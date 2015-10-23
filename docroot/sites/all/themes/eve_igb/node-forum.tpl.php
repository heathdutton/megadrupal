<?php
  if (!_is_forum()) {
    include('node.tpl.php');
    return;
  }
  $curr_user = user_load(array('uid' => $userid));
  $sig = $curr_user->signature;
?>
<div class="comment forum-comment comment-<?php print $row_class; print $comment->new ? ' comment-new forum-comment-new' : ''; ?>">
<?php
echo '<hr>';

print '[ '.$name . ' | ' . t('Posts:') . ' ' . $posts . ' | '. t('Joined:') . ' ' . $joined;
if ($comment->new) {
  print $new;
}
print ' ]<br /><h4>' . check_plain($comment->subject) . '</h4>';
print $content;
if ($sig) {
print '--<br />'.check_markup($sig);
}
?>
<br class="clear" />
<div class="links"><?php print $submitted . ' ' . $links ?></div>

</div>
<br class="clear" />
