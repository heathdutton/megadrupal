<?php
/**
 * @file
 * Template to display a view as a slider pro instance.
 */

$rows = $variables['rows'];
$id = $variables['id'];
$field_classes = $variables['field_classes'];
$field_labels = $variables['field_labels'];
$use_thumbnails = $variables['use_thumbnails'];
?>
<div class="slider-pro" id="slider-pro-<?php print $id; ?>">
  <div class="sp-slides">
    <?php foreach ($rows as $num => $row): ?>
      <div class="sp-slide">
        <?php foreach ($row['slide'] as $field_name => $field): ?>
          <div  class="<?php print implode(' ', $field_classes[$field_name][$num]); ?>">
            <?php if (!empty($field_labels[$field_name][$num])): ?>
              <div class="field-label">
                <label><?php print $field_labels[$field_name][$num]; ?></label>
              </div>
            <?php endif; ?>
            <div class="field-content">
              <?php print $field; ?>
            </div>
          </div>
        <?php endforeach; ?>
        <?php if ($use_thumbnails): ?>
          <div class="sp-thumbnail">
            <?php foreach ($row['thumb'] as $field_name => $field): ?>
              <div  class="<?php print implode(' ', $field_classes[$field_name][$num]); ?>">
                <?php if (!empty($field_labels[$field_name][$num])): ?>
                  <div class="field-label">
                    <label><?php print $field_labels[$field_name][$num]; ?></label>
                  </div>
                <?php endif; ?>
                <div class="field-content">
                  <?php print $field; ?>
                </div>
              </div>
            <?php endforeach; ?>
          </div>
        <?php endif; ?>
        <?php foreach ($row['layers'] as $layer): ?>
          <div class="sp-layer sp-padding <?php print $layer['background']; ?>" 
               data-position="<?php print $layer['data_position'] ?>"  
               data-show-transition="<?php print $layer['data_show_transition'] ?>" 
               data-hide-transition="<?php print $layer['data_hide_transition'] ?>" 
               data-show-delay="<?php print $layer['data_show_delay'] ?>"
               data-stay-duration="<?php print $layer['data_stay_duration'] ?>">
            <?php foreach ($layer['fields'] as $field_name => $field): ?>
              <div class="<?php print implode(' ', $field_classes[$field_name][$num]); ?>">
               <?php if (!empty($field_labels[$field_name][$num])): ?>
                 <div class="field-label">
                   <label><?php print $field_labels[$field_name][$num]; ?></label>
                 </div>
               <?php endif; ?>     
               <div class="field-content">   
                <?php print $field; ?>
               </div>    
              </div>
           <?php endforeach; ?> 
          </div> 
        <?php endforeach; ?>
      </div> 
    <?php endforeach; ?>
  </div>
</div>
