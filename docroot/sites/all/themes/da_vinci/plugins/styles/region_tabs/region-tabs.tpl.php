<?php
/**
 * @file panels-pane.tpl.php
 * Plantilla de flexiblock
 *
 * Variables disponibles
 * - $extra_class: clase base ('flexiblock-block') mas todas las que el usuario haya añadido + clases añadidas en el proceso de renderizado
 * - $title: Titulo del bloque
 * - $title_tag: Etiqueta html a utilizar para el texto
 * - $icono: Imagen del icono si se seleccionó alguna
 */
?>
<div id="<?php print $id; ?>" class="tabs-wrapper">

  <ul class="nav nav-tabs" role="tablist">
    <?php foreach ($tabs as $k_tab => $tab): ?>
      <li>
        <a href="#<?php print $k_tab; ?>" role="tab" data-toggle="tab"><?php print $tab; ?></a>
      </li>
    <?php endforeach ?>
  </ul>

  <div class="tab-content">
    <?php foreach ($content as $k_item => $item): ?>
      <div id="<?php print 'tab_' . $k_item; ?>" class="content tab-pane">
        <?php print $item; ?>
      </div>
    <?php endforeach ?>

  </div>

</div>
