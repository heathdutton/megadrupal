<?php
/**
 * @file
 * Render the modules documentation page.
 */
?>
<div>
  <div>
    <div id = drupal-image>
      <img src = "<?php print url() . drupal_get_path('module', 'developer_docs')?>/images/drupal.png" />
    </div>
    <div class = "mod-title">
      <h3 class="mod-comp"><?php print $vars['module'];?> - A Brief Module Description</h3>
    </div>
    <div class='mod-info'>
      <?php if (!empty($vars['module_info']['created'])) : ?>
        <p>This module was created on <b><?php print $vars['module_info']['created'];?></b>
        and has been downloaded <b><?php print $vars['module_info']['downloads'];?></b> times </p>
        <p>
          <b>Development Status: </b><?php print $vars['module_info']['develop_status'];?>&nbsp;&nbsp;
          <b>Maintenance Status: </b><?php print $vars['module_info']['maintain_status'];?>
        </p>
      <?php elseif (!empty($vars['module_info']['description'])) : ?>
        <p><b>Description: </b><?php print $vars['module_info']['description'];?></p>
      <?php endif; ?>
    </div>
  </div>
  <div class='mod-description'>
    <?php if (!empty($vars['module_info']['description']) && !empty($vars['module_info']['created'])) : ?>
      <p><b>Description: </b><?php print $vars['module_info']['description'];?></p>
    <?php endif; ?>
    <?php if (!empty($vars['module_info']['readme'])) : ?>
      <fieldset id="readme-text" class="collapsible form-wrapper collapsed">
        <p><b>&nbsp;Read me file provided with <?php print $vars['module']?> Module </b></p>
        <legend>
          <span class="fieldset-legend">Readme file : <?php print $vars['module'];?></span>
        </legend>
        <div class="fieldset-wrapper"><?php print nl2br($vars['module_info']['readme']);?></div>
      </fieldset>
    <?php endif; ?>
  </div>
  <div class='table-contents'>
  <h4>Information about <?php print $vars['module'];?> Module</h4>
  <ul>
  <?php foreach ($vars['index'] as $id => $title):?>
    <li><a href="#<?php print $id?>"><?php print $title ?></a></li>
  <?php endforeach; ?>
  </ul>
  </div>
  <?php foreach ($vars['entities'] as $id => $value) :?>
    <?php if ($id != 'hooks' && !empty($value)) : ?>
      <div>
        <h3 class="mod-comp" id="<?php print $id?>"><?php print $vars['index'][$id] ?></h3>
        <?php print $value;?>
      </div>
    <?php elseif ($id == 'hooks' && !empty($value['core']) || !empty($value['contrib'])) :?>
      <div>
      <div><h3 id='hooks' class='mod-comp'>Hooks</h3>
      <?php if (!empty($value['core'])) : ?>
        <p><b>Core Hooks:</b></p>
        <?php print $value['core']; ?>
      <?php  endif; ?>
      <?php if (!empty($value['contrib'])) : ?>
        <p><b>Contributed Hooks:</b></p>
        <?php print $value['contrib']; ?>
      <?php  endif; ?>
      </div>
    <?php endif; ?>
  <?php endforeach; ?>
  <div><h3 id='facts' class='mod-comp'>Quick Facts about the Module</h3>
    <ul>
    <?php if (!empty($vars['module_info']['depends'])) : ?>
      <li><b>Dependencies: </b><?php print $vars['module_info']['depends'] ?></li>
    <?php endif; ?>
    <?php if (!empty($vars['module_info']['path'])) : ?>
      <li><b>Module path: </b><?php print $vars['module_info']['path'] ?></li>
    <?php endif; ?>
    <?php if (empty($vars['module_info']['readme'])) : ?>
      <li>No Readme file is provided with this module</li>
    <?php endif; ?>
    <?php foreach ($vars['void_elements'] as $key => $element) : ?>
        <?php if ($key == 'hook_api') : ?>
          <li>No Hook is defined by this module</li>
        <?php else: ?>
          <li>No <?php print $element ?> are created by this module</li>
        <?php endif; ?>
    <?php endforeach;?>
    </ul>
  </div>
</div>
