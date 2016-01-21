<div class="abs-verse"><?php
foreach ($verses as $verse) {
  print '<div class="abs-verse">';
  print '<p><a href="?viewid=' . substr( $verse->id, 0, strrpos( $verse->id, '.' ) ) . '#' . $verse->id . '">' . $verse->version . '</a></p>';
  print '<blockquote>';
  print $verse->text;
  print '</blockquote>';
  print '</div>';
}
?></div>