<ul>
  <?php
  foreach ($tabs as $tab) {
    if ($tab['active']) {
      print '<li class="active"><a href="' . $tab['path'] . '" >' . $tab['label'] . '</a></li>';
    }
    else {
      print '<li><a href="' . $tab['path'] . '">' . $tab['label'] . '</a></li>';
    }
  }
?>
</ul>
