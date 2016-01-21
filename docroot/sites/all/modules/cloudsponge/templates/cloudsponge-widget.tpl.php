<!-- Any link with a class="cs_import" will start the import process -->
<p><?php print t("Select people from your address book:") ?></p>
<span>
  <?php foreach ($providers as $provider => $link): ?>
    <a href="#" onclick="return cloudsponge.launch('<?php print $provider ?>');">
      <img src="<?php print $link ?>" alt="<?php print $provider?>">
    </a>
  <?php endforeach ?>
</span>
