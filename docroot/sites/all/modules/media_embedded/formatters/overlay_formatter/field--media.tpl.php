<?php

/**
 * @file field.tpl.php
 * Default template implementation to display the value of a field.
 *
 * This file is not used and is here as a starting point for customization only.
 * @see theme_field()
 *
 * Available variables:
 * - $items: An array of field values. Use render() to output them.
 * - $label: The item label.
 * - $label_hidden: Whether the label display is set to 'hidden'.
 * - $classes: String of classes that can be used to style contextually through
 *   CSS. It can be manipulated through the variable $classes_array from
 *   preprocess functions. The default values can be one or more of the
 *   following:
 *   - field: The current template type, i.e., "theming hook".
 *   - field-name-[field_name]: The current field name. For example, if the
 *     field name is "field_description" it would result in
 *     "field-name-field-description".
 *   - field-type-[field_type]: The current field type. For example, if the
 *     field type is "text" it would result in "field-type-text".
 *   - field-label-[label_display]: The current label position. For example, if
 *     the label position is "above" it would result in "field-label-above".
 *
 * Other variables:
 * - $element['#object']: The entity to which the field is attached.
 * - $element['#view_mode']: View mode, e.g. 'full', 'teaser'...
 * - $element['#field_name']: The field name.
 * - $element['#field_type']: The field type.
 * - $element['#field_language']: The field language.
 * - $element['#field_translatable']: Whether the field is translatable or not.
 * - $element['#label_display']: Position of label display, inline, above, or
 *   hidden.
 * - $field_name_css: The css-compatible field name.
 * - $field_type_css: The css-compatible field type.
 * - $classes_array: Array of html class attribute values. It is flattened
 *   into a string within the variable $classes.
 *
 * @see template_preprocess_field()
 * @see theme_field()
 */
?>
<?php if(!empty($items)) : ?>
<?php
//dpm($items);
//dpm($element);

GLOBAL $ul_ids;

$ul_ids = (empty($ul_ids)) ? array() : $ul_ids;

do {
	$new_id = mt_rand();
} while (in_array($new_id, $ul_ids));

$ul_class= "media-embedded-" . $new_id;


$entity = $element['#object'];
$entity_type = $element['#entity_type'];
$field_name = $element['#field_name'];

$media_items = field_get_items($entity_type, $entity, $field_name);

if (empty($media_items)) {
	$media_items = $element['#object']->$field_name;
}

$counter = 0;
if(!empty($media_items)) {
	foreach ($media_items as $delta => $item) {
		$data = unserialize($item['data']);
		$items[$delta]['#media_embedded_counter'] = $data->media_embedded_counter;
		$items[$delta]['#media_embedded_href'] = $data->media_embedded_href;
		$items[$delta]['#media_embedded_rel'] = "shadowbox[media-embedded-". $new_id . "];" . $data->media_embedded_rel;
		$counter++;
	}
}

switch ($counter) {
	case 1:
		$skin = 'media_embedded_1';
		$options = array(
			'skin' => $skin,
			'scroll' => 0,
			'wrap' => 'null',
			'size' => '3',
		);
		break;

	case 2:
		$skin = 'media_embedded_2';
		$options = array(
			'skin' => $skin,
			'scroll' => 0,
			'wrap' => 'null',
			'size' => '3',
		);
		break;

	case 3:
		$skin = 'media_embedded_3';
		$options = array(
			'skin' => $skin,
			'scroll' => 0,
			'wrap' => 'null',
			'size' => '3',
		);
		break;

	default:
		$skin = 'media_embedded';
		$options = array(
			'skin' => $skin,
			'visible' => 3,
			'scroll' => 2,
			'wrap' => 'circular',
			'size' => '3',
			'easing' => 'easeInOutCubic',
		);
		drupal_add_js(drupal_get_path('module', 'media_embedded_overlay_formatter').'/skin/jquery.easing.1.2.js', 'file');
		break;
}

?>


<div class="<?php print $classes; ?> clearfix"
<?php print $attributes; ?>>
	<?php if (!$label_hidden) : ?>
	<div class="field-label" <?php print $title_attributes; ?>>
	<?php print $label ?>
		:&nbsp;
	</div>
	<?php endif; ?>
	<div class="field-items" <?php print $content_attributes; ?>>
		<ul
			class="<?php print $ul_class; ?> jcarousel-skin-<?php print $skin; ?>">
			<?php foreach ($items as $delta => $item) : ?>
			<li>
				<div class="field-item <?php print $delta % 2 ? 'odd' : 'even'; ?>"
				<?php print $item_attributes[$delta]; ?>>
					<a href="<?php print $item['#media_embedded_href']; ?>"
						rel='<?php print $item['#media_embedded_rel']; ?>'> [<?php print $item['#media_embedded_counter'];	?>]
						<?php print render($item); ?> </a>
				</div>
			</li>
			<?php endforeach; ?>
		</ul>
		<?php jcarousel_add($ul_class, $options); ?>
	</div>

</div>

<?php endif; ?>