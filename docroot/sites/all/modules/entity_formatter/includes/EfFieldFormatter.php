<?php

/**
 * Base class for Entity fields. Extend this class to implement custom field
 * formatter.
 */
class EfFieldFormatter implements IteratorAggregate, Countable {
  protected $property; // EntityStructureWrapper
  protected $value; // Array of properties of this field

  protected $isMultivalue = FALSE;
  protected $delta = 0;

  /**
   * Constructor.
   *
   * @param EntityMetadataWrapper $property
   *   The value of this field wrapped by an EntityMetadataWrapper.
   *
   * @param bool $isMultivalue
   *  Whether this field has a cardinality greater than one.
   */
  public function __construct($property, $isMultivalue) {
    $this->property = $property;

    if ($isMultivalue) {
      // It is already an array.
      $this->value = $this->property->value();
    }
    else {
      // Build an array with one item.
      $this->value = array($this->property->value());
    }

    $this->isMultivalue = $isMultivalue;
  }

  /**
   * Returns the current field item as a raw array.
   *
   * @return array|string
   */
  public function value() {
    return $this->value[$this->delta];
  }

  /**
   * Returns all raw field items in an array.
   *
   * @return array
   */
  public function values() {
    return $this->value;
  }

  /**
   * Set the current field delta.
   *
   * @return array
   */
  public function setDelta($delta) {
    if ($this->isDeltaValid($delta)) {
      $this->delta = $delta;
    }
    else {
      // TODO warning. delta is not valid.
    }

    return $this;
  }

  /**
   * Checks if the given or current field delta exists.
   *
   * @param null $delta
   * @return bool
   */
  public function isDeltaValid($delta = NULL) {
    $delta = empty($delta) ? $this->delta : $delta;

    return isset($this->value[$delta]);
  }

  /**
   * Check if current user is allowed to view this field.
   *
   * Can be needed if a module like field_permissions is in use.
   *
   * @return bool True, if current user is allowed to view this field.
   */
  public function isAccessible() {
    return $this->property->access('view');
  }

  /**
   * Implementing IteratorAggregate interface.
   *
   * If we wrap a list, we return an iterator over the data list.
   *
   * @return EfFieldFormatterIterator
   */
  public function getIterator() {
    return new EfFieldFormatterIterator($this);
  }

  /**
   * Implementing Countable interface.
   *
   * @return int The number of items of this field.
   */
  public function count() {
    return count($this->value);
  }
}

/**
 * Allows to easily iterate over field items.
 */
class EfFieldFormatterIterator implements Iterator {

  protected $position = 0;
  protected $wrapper;

  public function __construct(EfFieldFormatter $wrapper) {
    $this->wrapper = $wrapper;
  }

  function rewind() {
    $this->position = 0;
    $this->wrapper->setDelta($this->position);
  }

  function current() {
    return $this->wrapper;
  }

  function key() {
    return $this->position;
  }

  function next() {
    $this->position++;
    $this->wrapper->setDelta($this->position);
  }

  function valid() {
    return $this->wrapper->isDeltaValid($this->position);
  }

}

/**
 * Provides formatting methods for <code>list\_boolean</code>, <code>list\_float</code>, <code>list\_integer</code> and <code>list_\text</code> field types.
 */
class EfListFormatter extends EfFieldFormatter {
  /**
   * Returns the current field item key.
   *
   * @return string
   */
  public function key() {
    return $this->value[$this->delta];
  }

  /**
   * Returns the current field item label.
   *
   * @return string
   */
  public function label() {
    return $this->property[$this->delta]->label();
  }

  public function __toString() {
    return $this->label();
  }
}

/**
 * Provides formatting methods for <code>text</code>, <code>text\_long</code> and <code>text\_with_summary</code> field types.
 */
class EfTextFormatter extends EfFieldFormatter {
  /**
   * @var int Number of characters after which to truncate.
   */
  protected $truncate = 300;

  /**
   * Sets the number of characters after which to truncate.
   *
   * @param int $truncate Number of characters after which to truncate.
   * @return $this
   */
  public function setTruncate($truncate) {
    $this->truncate = $truncate;
    return $this;
  }

  /**
   * Returns the textual value of this field at the current delta.
   *
   * @return string The textual value of this field at the current delta.
   */
  public function text() {
    $item = $this->value[$this->delta];
    if (is_array($item)) {
      return $item['safe_value'];
    }
    else {
      return check_plain($item);
    }
  }

  /**
   * Returns the summary of this text or a truncated version of the text.
   * For truncating the core text_summary function is used; this function uses
   * the character count only as a rough guideline.
   *
   * If you need accurate truncating at a given char count, use the truncate method.
   *
   * @param int $truncate (Optional) Number of characters after which to truncate. If not specified the value of member variable is taken.
   * @return string The summary of this text or a truncated version of the text.
   */
  public function summary($truncate = 0) {
    $item = $this->value[$this->delta];
    $text = $this->text();

    if (!empty($item['safe_summary'])) {
      return $item['safe_summary'];
    }
    else {
      $format = isset($item['format']) ? $item['format'] : NULL;
      $trimLength = $truncate > 0 ? $truncate : $this->truncate;
      return text_summary($text, $format, $trimLength);
    }
  }

  /**
   * Hard cuts the string after a given character count and adds an ellipsis.
   * Any HTML is stripped of.
   *
   * For more sensible but unpredictable truncating use the summary method.
   *
   * @param int $truncate (Optional) Number of characters after which to truncate. If not specified the value of member variable is taken.
   * @param string $ellipsis A string to be added at the end. Defaults to the "&hellip;".
   * @return string
   */
  public function truncate($truncate = 0, $ellipsis = '&hellip;') {
    $text = $this->text();
    $trimLength = $truncate > 0 ? $truncate : $this->truncate;

    // Strip out any HTML.
    $text = strip_tags($text);

    if (drupal_strlen($text) <= $trimLength) {
      // Return the whole text if it is shorter then the trim length.
      return $text;
    }

    // Do the trim!
    $text = drupal_substr($text, 0, $trimLength);

    // Add the ellipsis.
    if (drupal_strlen($ellipsis)) {
      // If we're adding an ellipsis, remove any trailing periods.
      $text = rtrim($text, '.');

      $text .= $ellipsis;
    }

    return $text;
  }

  public function __toString() {
    return $this->text();
  }

}

/**
 * Provides formatting methods for <code>image</code> field types.
 */
class EfImageFormatter extends EfFieldFormatter {
  /**
   * @var string This image style which is used when printing this image.
   */
  protected $imageStyle;

  /**
   * Set the image style which is used when printing this image.
   *
   * @param $imageStyle
   * @return $this
   */
  public function setImageStyle($imageStyle) {
    $this->imageStyle = $imageStyle;
    return $this;
  }

  /**
   * Returns the URL for this image.
   *
   * @return string
   */
  public function src() {
    if (!$this->isDeltaValid()) {
      return '';
    }

    if ($this->imageStyle) {
      return image_style_url($this->imageStyle, $this->value[$this->delta]['uri']);
    }
    else {
      return file_create_url($this->value[$this->delta]['uri']);
    }
  }

  /**
   * Returns an <img> tag for this image.
   *
   * @return string
   */
  public function img() {
    if (!$this->isDeltaValid()) {
      return '';
    }

    $variables = $this->value[$this->delta];

    $variables += array(
      'style_name' => $this->imageStyle,
      'path' => $this->value[$this->delta]['uri'],
    );

    if ($this->imageStyle) {
      return theme('image_style', $variables);
    }
    else {
      return theme('image', $variables);
    }
  }

  /**
   * Returns the alt attribute for this image.
   *
   * @return string
   */
  public function alt() {
    if (!$this->isDeltaValid()) {
      return '';
    }

    // Note: Text is already escaped here.
    return $this->value[$this->delta]['alt'];
  }

  /**
   * Returns the title attribute for this image.
   *
   * @return string
   */
  public function title() {
    if (!$this->isDeltaValid()) {
      return '';
    }

    // Note: Text is already escaped here.
    return $this->value[$this->delta]['title'];
  }

  public function __toString() {
    return $this->img();
  }
}

/**
 * Provides formatting methods for <code>file</code> field types.
 */
class EfFileFormatter extends EfFieldFormatter {
  public function __construct($property, $isMultivalue) {
    parent::__construct($property, $isMultivalue);

    // Respect the "display" flag on files.
    foreach($this->value as $key => $item) {
      if (!(bool) $item['display']) {
        unset($this->value[$key]);
      }
    }
  }

  /**
   * Returns the filename.
   *
   * @return string
   */
  public function name() {
    if (!$this->isDeltaValid()) {
      return '';
    }
    // Note: Text is already escaped here.
    return $this->value[$this->delta]['filename'];
  }

  /**
   * Returns the textual description if it is available.
   *
   * @return string
   */
  public function description() {
    if (!$this->isDeltaValid() || !isset($this->value[$this->delta]['description'])) {
      return '';
    }
    // Note: Text is already escaped here.
    return $this->value[$this->delta]['description'];
  }

  /**
   * Return a formatted filesize.
   *
   * @return string
   */
  public function filesize() {
    if (!$this->isDeltaValid()) {
      return '';
    }

    return format_size($this->value[$this->delta]['filesize']);
  }

  /**
   * Return a filetype.
   *
   * @return string
   */
  public function filetype() {
    if (!$this->isDeltaValid()) {
      return '';
    }
    $info = pathinfo($this->value[$this->delta]['uri']);

    return $info['extension'];
  }

  /**
   * Returns the URL for this file.
   *
   * @return string
   */
  public function url() {
    if (!$this->isDeltaValid()) {
      return '';
    }

    return file_create_url($this->value[$this->delta]['uri']);
  }

  /**
   * Returns link to this file generated with theme\_file\_link
   *
   * @return string
   */
  public function link() {
    if (!$this->isDeltaValid()) {
      return '';
    }

    $variables = array(
      'file' => (object) $this->value[$this->delta],
    );

    return theme('file_link', $variables);
  }

  public function __toString() {
    return $this->link();
  }
}

/**
 * Provides formatting methods for <code>taxonomy\_term\_reference</code>, <code>field\_collection</code> and <code>entityreference</code> field types.
 */
class EfEntityReferenceFormatter extends EfFieldFormatter {
  protected $viewMode = 'full';
  protected $entityType;

  public function __construct($property, $isMultivalue) {
    parent::__construct($property, $isMultivalue);

    $type = $this->property->type();
    if (entity_property_list_extract_type($type)) {
      $this->entityType = entity_property_list_extract_type($type);
    }
    else {
      $this->entityType = $type;
    }
  }

  /**
   * Sets the view mode which is used to render this entity.
   *
   * @param $viewMode
   * @return $this
   */
  public function setViewMode($viewMode) {
    $this->viewMode = $viewMode;
    return $this;
  }

  /**
   * Returns a EfEntityFormatter for the specific entity type and bundle to access single fields on this entity.
   *
   * @return EfEntityFormatter
   */
  public function getFormatter() {
    if (!$this->isDeltaValid()) {
      return;
    }

    $entity = $this->value[$this->delta];

    $wrapped_entity = entity_metadata_wrapper($this->entityType, $entity);
    $bundle = $wrapped_entity->getBundle();

    $name = entity_formatter_determine_class_name($this->entityType, $bundle);

    if (class_exists($name)) {
      return new $name($entity);
    }
    else {
      return new EfEntityFormatter($this->entityType, $entity);
    }

  }

  /**
   * Renders an entity using the set view mode.
   *
   * @return string
   */
  public function render() {
    if (!$this->isDeltaValid()) {
      return '';
    }

    $render_array = entity_view($this->entityType, array($this->value[$this->delta]), $this->viewMode);
    return drupal_render($render_array);
  }

  /**
   * Returns the path to the detail page of this entity.
   *
   * @return string
   */
  public function path() {
    if (!$this->isDeltaValid()) {
      return '';
    }

    $path = entity_uri($this->entityType, $this->value[$this->delta]);
    return $path['path'];
  }

  /**
   * Returns an URL to the detail page of this entity.
   *
   * @param array $options
   * @return string
   */
  public function url($options = array()) {
    if (!$this->isDeltaValid()) {
      return '';
    }

    $url = url($this->path(), $options);

//    return check_url($url); // Entity URL can be assumed to be save?
    return $url;
  }

  /**
   * Returns the label of this entity.
   *
   * @return string
   */
  public function label() {
    if (!$this->isDeltaValid()) {
      return '';
    }

    $label = entity_label($this->entityType, $this->value[$this->delta]);
    return check_plain($label);
  }

  /**
   * Returns the node title wrapped in a link to the detail page of this entity.
   *
   * @return string
   */
  public function linkedLabel() {
    if (!$this->isDeltaValid()) {
      return '';
    }

    $label = entity_label($this->entityType, $this->value[$this->delta]);
    return l($label, $this->path());
  }

  public function __toString() {
    return $this->render();
  }
}

/**
 * Provides formatting methods for <code>link\_field</code> field types.
 */
class EfLinkFormatter extends EfFieldFormatter {

  /**
   * Returns the URL of the current link field item.
   *
   * @return string
   */
  public function url() {
    if (!$this->isDeltaValid()) {
      return '';
    }

    $url = url($this->value[$this->delta]['url']);

    return check_url($url);
  }

  /**
   * Returns a rendered link of the current link field item.
   *
   * @return string
   */
  public function link() {
    if (!$this->isDeltaValid()) {
      return '';
    }

    $item = $this->value[$this->delta];

    // This seems not to be needed as the hook_field_prepare_view is called anyhow.
//    $info = $this->property->info();
//    $parent = $info['parent']->value();
//    _link_sanitize($item, $this->delta, $info['field'], $info['instance'], $parent);

    $link_options = $item;
    unset($link_options['title']);
    unset($link_options['url']);

    $title = !empty($item['title']) ? $item['title'] : $item['display_url'];

    return l($title, $item['url'], $link_options);
  }

  public function __toString() {
    return $this->link();
  }
}

/**
 * Provides formatting methods for <code>date</code>, <code>datetime</code> and <code>datestamp</code> field types.
 */
class EfDateFormatter extends EfFieldFormatter {
  /**
   * @var string A Drupal date format name
   */
  protected $format = 'medium';

  /**
   * @var string A PHP date format string.
   */
  protected $customFormat = 'd.F Y H:i';

  /**
   * Set the date format name which will be used for printing the date.
   *
   * @param string $format A Drupal date format name
   * @return $this
   */
  public function setFormat($format) {
    $this->format = $format;
    return $this;
  }

  /**
   * Set a custom PHP date format string which will be used for printing the date.
   *
   * @param string $format A PHP date format string.
   * @return $this
   */
  public function setCustomFormat($format) {
    $this->customFormat = $format;
    return $this;
  }

  /**
   * Checks if an enddate is specified for the current date field item.
   *
   * @return bool
   */
  public function hasEnddate() {
    if (!$this->isDeltaValid()) {
      return FALSE;
    }

    $item = $this->value[$this->delta];

    return (isset($item['value2']) && $item['value'] != $item['value2']);
  }

  /**
   * Returns a timestamp for current date field item.
   *
   * @return int
   */
  public function date() {
    if (!$this->isDeltaValid()) {
      return '';
    }

    $item = $this->value[$this->delta];
    if (is_array($item)) {
      $date_string = $item['value'];
      return $this->dateArrayToTimestamp($date_string, $item);
    }
    else {
      return $item;
    }
  }

  /**
   * Returns a timestamp of the enddate for current date field item.
   *
   * @return int
   */
  public function enddate() {
    if (!$this->hasEnddate()) {
      return '';
    }

    $item = $this->value[$this->delta];

    $date_string = $item['value2'];
    return $this->dateArrayToTimestamp($date_string, $item);
  }

  /**
   * Return the duration from start to enddate as a string using format_interval.
   *
   * @param int $granularity
   * @return string
   */
  public function duration($granularity = 2) {
    if (!$this->isDeltaValid() || !$this->hasEnddate()) {
      return '';
    }
    $interval = $this->enddate() - $this->date();
    return format_interval($interval, $granularity);
  }

  /**
   * Returns a date formatted which the given or currently set date format.
   *
   * @param null $format (Optional) A Drupal date format name.
   * @return string
   */
  public function formatted($useEnddate = FALSE) {
    if (!$this->isDeltaValid() || ($useEnddate && !$this->hasEnddate())) {
      return '';
    }

    $date_getter = $useEnddate ? 'enddate' : 'date';
    $date = $this->$date_getter();

    return format_date($date, $this->format);
  }

  /**
   * Returns a date formatted which the given or currently set PHP date format string.
   *
   * @param null $customFormat (Optional) A PHP date format string to be used.
   * @return mixed
   */
  public function customFormatted($useEnddate = FALSE) {
    if (!$this->isDeltaValid()) {
      return '';
    }

    $date_getter = $useEnddate ? 'enddate' : 'date';
    $date = $this->$date_getter();

    return format_date($date, 'custom', $this->customFormat);
  }

  protected function dateArrayToTimestamp($date_string, $item) {
    $tz = $item['timezone']; // in which timezone is the saved value?
    $tz_db = $item['timezone_db']; // which timezone should be used for display?

    $date = new DateTime($date_string, new DateTimeZone($tz));
    $date->setTimezone(new DateTimeZone($tz_db));

    return $date->getTimestamp();
  }

  public function __toString() {
    return $this->formatted();
  }

}

/**
 * Provides formatting methods for <code>addressfield</code> field type.
 */
class EfAddressFieldFormatter extends EfFieldFormatter {

  protected $handlers;

  public function __construct($property, $isMultivalue) {
    parent::__construct($property, $isMultivalue);

    // Get field info and use the addressfield handler from the widget by default.
    $info = $property->info();
    $this->handlers = $handlers = $info['instance']['widget']['settings']['format_handlers'];
  }

  /**
   * Set formatting handlers for this addressfield.
   *
   * @param $handlers
   * @return $this
   */
  public function setHandlers($handlers) {
    // Use the values also as keys.
    $handlers = array_combine($handlers, $handlers);

    $this->handlers = $handlers;
    return $this;
  }

  /**
   * Returns an formatted address. By default the formatting handlers from the widget are used.
   *
   * @return string
   */
  public function address() {
    if (!$this->isDeltaValid()) {
      return '';
    }

    $item = $this->value[$this->delta];
    $info = $this->property->info();

    $context = array(
      'mode' => 'render',
      'field' => $info['field'],
      'instance' => $info['instance'],
      'langcode' => $info['langcode'],
      'delta' => $this->delta,
    );
    $render_array = addressfield_generate($item, $this->handlers, $context);
    return drupal_render($render_array);
  }

  /**
   * Returns the country name of this address.
   *
   * @return string
   */
  public function country() {
    if (!$this->isDeltaValid()) {
      return '';
    }

    $item = $this->value[$this->delta];
    $countries = _addressfield_country_options_list();
    return $countries[$item['country']];
  }

  /**
   * Returns the city of this address.
   *
   * @return string
   */
  public function city() {
    if (!$this->isDeltaValid()) {
      return '';
    }

    $item = $this->value[$this->delta];
    return check_plain($item['locality']);
  }

  /**
   * Returns the postal code of this address.
   *
   * @return string
   */
  public function postalCode() {
    if (!$this->isDeltaValid()) {
      return '';
    }

    $item = $this->value[$this->delta];
    return check_plain($item['postal_code']);
  }

  /**
   * Returns the street of this address.
   *
   * @return string
   */
  public function street() {
    if (!$this->isDeltaValid()) {
      return '';
    }

    $item = $this->value[$this->delta];
    return check_plain($item['thoroughfare']);
  }

  /**
   * Returns the name line of this address.
   *
   * @return string
   */
  public function name() {
    if (!$this->isDeltaValid()) {
      return '';
    }

    $item = $this->value[$this->delta];
    return check_plain($item['name_line']);
  }

  /**
   * Returns the company of this address.
   *
   * @return string
   */
  public function company() {
    if (!$this->isDeltaValid()) {
      return '';
    }

    $item = $this->value[$this->delta];
    return check_plain($item['organisation_name']);
  }

  public function __toString() {
    return $this->address();
  }
}

/**
 * Provides formatting methods for <code>viewfield</code field type.
 *
 * NOTE: Currently you need to patch viewfield module in order to make this work.
 *   See https://www.drupal.org/node/2451779
 */
class EfViewFieldFormatter extends EfFieldFormatter {
  /**
   * @return string
   */
  public function render() {
    if (!$this->isDeltaValid()) {
      return '';
    }

    $item = $this->value[$this->delta];

    list($view_name, $view_display) = explode('|', $item['vname'], 2);

    $view = views_get_view($view_name);

    if ($view && $view->access($view_display)) {
      // Override the view's path to the current path. Otherwise, exposed
      // views filters would submit to the front page.
      $view->override_path = current_path();

      // Get the parent and process the args.
      $info = $this->property->info();
      $parent = $info['parent'];
      $item['vargs'] = _viewfield_get_view_args($item['vargs'], $parent->type(), $parent->value());

      return $view->preview($view_display, $item['vargs']);
    }
  }

  public function __toString() {
    return $this->render();
  }
}

/**
 * Work in Progress.
 */
class EfFileEntityFormatter extends EfEntityReferenceFormatter {
  public function __construct($property, $isMultivalue) {
    parent::__construct($property, $isMultivalue);

    $this->entityType = 'file';

  }
  /**
   * Render an file entity with a view mode.
   *
   * @return string
   */
  public function render() {
    $item = $this->value[$this->delta];
    $file = file_load($item['fid']);
    //kpr($file);
    //$render_array = file_view($file, $this->viewMode);
    $render_array = entity_view($this->entityType, array($file), $this->viewMode);
    return drupal_render($render_array);
  }

}

