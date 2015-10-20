<?php
/**
 * @file
 * Contains entity classes for Shopify products.
 */

/**
 * Class ShopifyProduct
 */
class ShopifyProduct extends Entity {

  /**
   * The name of the product.
   * @var string
   */
  public $title;

  /**
   * The description of the product, complete with HTML formatting.
   * @var string
   */
  public $body_html;

  /**
   * A human-friendly unique string for the Product automatically generated from its title.
   * They are used by the Liquid templating language to refer to objects.
   * @var string
   */
  public $handle;

  /**
   * The unique numeric identifier for the product.
   * Product ids are unique across the entire Shopify system;
   * no two products will have the same id, even if they're from different shops.
   * @var int
   */
  public $product_id;

  /**
   * The unique numeric identifier for the product variant.
   * @var int
   */
  public $variant_id;

  /**
   * A categorization that a product can be tagged with,
   * commonly used for filtering and searching.
   * @var string
   */
  public $product_type;

  /**
   * The sales channels in which the product is visible.
   * @var string
   */
  public $published_scope;

  /**
   * The name of the vendor of the product.
   * @var string
   */
  public $vendor;

  /**
   * A categorization that a product can be tagged with,
   * commonly used for filtering and searching.
   * @var string
   */
  public $tags;

  /**
   * Custom properties that a shop owner can use to define product variants.
   * Multiple options can exist. Serialized array?
   *
   * @todo: What?
   * @var array
   */
  public $options;

  /**
   * The barcode, UPC or ISBN number for the product.
   * @var string
   */
  public $barcode;

  /**
   * The competitors prices for the same item.
   * @var double
   */
  public $compare_at_price;

  /**
   * Service who is doing the fulfillment. Valid values are: manual.
   * @var string
   */
  public $fulfillment_service;

  /**
   * The weight of the product variant in grams.
   * @var int
   */
  public $grams;

  /**
   * Specifies whether or not Shopify tracks the number of items in stock for this product variant.
   * Valid values are:
   *  - blank: Shopify does not track the number of items in stock for this product variant.
   *  - shopify: Shopify does track the number of items in stock for this product variant.
   * @var string
   */
  public $inventory_management;

  /**
   * Specifies whether or not customers are allowed to place an order for a product variant when it's out of stock.
   * Valid values are:
   *  - deny (default): Customers are not allowed to place orders for a product variant when it's out of stock.
   *  - continue: Customers are allowed to place orders for a product variatn when it's out of stock.
   * @var string
   */
  public $inventory_policy;

  /**
   * The number of items in stock for this product variant.
   * @var int
   */
  public $inventory_quantity;

  /**
   * The original stock level the client believes the product variant has.
   * This should be sent to avoid a race condition when the item being adjusted is simultaneously sold online.
   * @var int
   */
  public $old_inventory_quantity;

  /**
   * Custom product property names like "Size", "Color", and "Material".
   * Products are based on permutations of these options.
   * A product may have a maximum of 3 options. 255 characters limit each.
   * @var array
   */
  public $option_values;

  /**
   * The price of the product variant.
   * @var double
   */
  public $price;

  /**
   * Specifies whether or not a customer needs to provide a shipping address when placing an order for this product variant.
   * Valid values are:
   *  - true: Customer needs to supply a shipping address.
   *  - false: Customer does not need to supply a shipping address.
   * @var boolean
   */
  public $requires_shipping;

  /**
   * A unique identifier for the product in the shop.
   * @var string
   */
  public $sku;

  /**
   * Specifies whether or not a tax is charged when the product variant is sole.
   * @var boolean
   */
  public $taxable;

  /**
   * The weight of the product variant in the unit system specified with weight_unit.
   * @var double
   */
  public $weight;

  /**
   * The unit system that the product variant's weight is measure in.
   * The weight_unit can be either "g", "kg, "oz", or "lb".
   * @var string
   */
  public $weight_unit;

  /**
   * The date and time when the product was created.
   * @var int
   */
  public $created_at;

  /**
   * The date and time when the product was published.
   * @var int
   */
  public $published_at;

  /**
   * The date and time when the product was last modified.
   * @var int
   */
  public $updated_at;

  /**
   * Product variants.
   * @var ShopifyProduct[]
   */
  public $variants;

  /**
   * Creates a new ShopifyProduct entity.
   */
  public function __construct(array $values = array()) {
    // Handle shopify product and variant IDs.
    if (!empty($values)) {
      if (isset($values['id']) && (!isset($values['product_id']) || !$values['product_id'])) {
        $values['product_id'] = $values['id'];
      }
      elseif (isset($values['id']) && isset($values['product_id']) && $values['product_id']) {
        $values['variant_id'] = $values['id'];
      }
      unset($values['id']);
      if (empty($values['variant_id'])) {
        $values['variant_id'] = 0;
      }
      // Format variant option values.
      if (!isset($values['variants']) || !$values['variants']) {
        // This is a variant product.
        $values['option_values'] = $this->formatVariantOptions($values);
      }
      // Options should be stored as a serialized array.
      if (isset($values['options'])) {
        $values['options'] = serialize($values['options']);
      }
    }
    parent::__construct($values, 'shopify_product');
    if (!empty($values)) {
      $this->setTerms();
      $this->setImages();
    }
  }

  /**
   * Formats all date times into Unix timestamps.
   */
  private function formatTimestamps() {
    foreach (array('published_at', 'updated_at', 'created_at') as $field) {
      $this->{$field} = $this->formatTimestamp($this->{$field});
    }
  }

  /**
   * Format a date time as Unix timestamp.
   *
   * @param string $datetime
   *   Datetime string.
   *
   * @return int
   *   Unix timestamp.
   */
  private function formatTimestamp($datetime) {
    if (empty($datetime)) {
      return 0;
    }
    if (!is_numeric($datetime)) {
      $date = new DateTime($datetime);
      return $date->format('U');
    }
    // Already a timestamp.
    return $datetime;
  }

  /**
   * Set up the object instance on construction or unserializiation.
   */
  public function setUp() {
    if (!$this->variant_id) {
      // This is a regular product, need to load sub-variants.
      $this->variants = $this->loadVariants();
    }
    if (!is_array($this->options) && isset($this->options)) {
      $this->options = unserialize($this->options);
    }
    if (!is_array($this->option_values) && isset($this->option_values)) {
      $this->option_values = unserialize($this->option_values);
    }
    parent::setUp();
  }

  /**
   * Saves the entity.
   */
  public function save() {
    // Ensure that we are saving serialized array data.
    if (is_array($this->options)) {
      $this->options = serialize($this->options);
    }
    if (is_array($this->option_values)) {
      $this->option_values = serialize($this->option_values);
    }
    $this->formatTimestamps();
    return parent::save();
  }

  /**
   * Set images for this product.
   */
  function setImages() {
    $directory = file_build_uri('shopify_images');
    if (!file_prepare_directory($directory, FILE_CREATE_DIRECTORY)) {
      // If our directory doesn't exist and can't be created, use the default.
      $directory = NULL;
    }
    // @todo images on variants
    if ($this->variant_id == 0 && isset($this->images[0]['src'])) {
      foreach ($this->images as $product_image) {
        $file = system_retrieve_file($product_image['src'], $directory, TRUE, FILE_EXISTS_REPLACE);
        image_path_flush($file->uri);
        $files[] = (array) $file;
      }
      $this->wrapper()->shopify_product_images->set($files);
    }
  }

  /**
   * Set terms from tags for this product.
   */
  function setTerms() {
    // Reset terms for this products.
    $this->wrapper()->shopify_product_tags = array();
    foreach (array_reverse(explode(',', $this->tags)) as $tag) {
      $tag = trim($tag);
      if ($tag) {
        $term = taxonomy_get_term_by_name($tag, 'shopify_tags');
        if (!empty($term)) {
          $tid = reset($term)->tid;
        }
        else {
          $vocab = taxonomy_vocabulary_machine_name_load('shopify_tags');
          $term = new stdClass();
          $term->name = $tag;
          $term->vid = $vocab->vid;
          taxonomy_term_save($term);
          $tid = $term->tid;
        }
        $this->wrapper()->shopify_product_tags[] = $tid;
      }
    }
  }

  /**
   * Load variants for this product.
   *
   * @return ShopifyProduct[]
   *   ShopifyProducts that are variants of this product.
   */
  private function loadVariants() {
    $variants = array();
    $query = new EntityFieldQuery();
    $result = $query->entityCondition('entity_type', 'shopify_product')
      ->propertyCondition('variant_id', 0, '!=')
      ->propertyCondition('product_id', $this->product_id)
      ->execute();
    if (isset($result['shopify_product']) && $result['shopify_product']) {
      $variants = entity_load('shopify_product', array_keys($result['shopify_product']));
    }
    return $variants;
  }

  /**
   * Formats variant option values.
   *
   * @param array $values
   *
   * @return string
   *   Serialized array of option values.
   */
  private function formatVariantOptions(array $values = array()) {
    $option_values = array();
    foreach ($values as $key => $value) {
      if (strpos($key, 'option') !== FALSE) {
        $option_values[] = $value;
      }
    }
    return serialize($option_values);
  }

}

/**
 * Class ShopifyProductController
 */
class ShopifyProductController extends EntityAPIController {

  /**
   * Implements EntityAPIControllerInterface.
   */
  public function view($entities, $view_mode = 'full', $langcode = NULL, $page = NULL) {
    // Don't let someone view a specific variant page.
    if ($view_mode == 'full' && $page == TRUE) {
      foreach ($entities as $entity) {
        if ($entity->variant_id) {
          if ($parent = shopify_product_load_single($entity->product_id)) {
            drupal_goto('product/' . $parent->id, array('query' => array('variant_id' => $entity->variant_id)));
          }
        }
      }
    }
    return parent::view($entities, $view_mode, $langcode, $page);
  }

  /**
   * Implements EntityAPIController.
   */
  public function buildContent($entity, $view_mode = 'full', $langcode = NULL, $content = array()) {
    $extra_fields = field_extra_fields_get_display('shopify_product', 'shopify_product', $view_mode);
    if ($extra_fields['add_to_cart']['visible']) {
      $content['add_to_cart'] = drupal_get_form('shopify_add_to_cart_form', $entity);
    }
    return parent::buildContent($entity, $view_mode, $langcode, $content);
  }

  /**
   * Renders a single entity property.
   * //   */
  protected function renderEntityProperty($wrapper, $name, $property, $view_mode, $langcode, &$content) {
    $info = $property->info();

    $content[$name] = array(
      // Overrridden to hide labels by default
      '#label_hidden' => FALSE,
      '#label' => $info['label'],
      '#entity_wrapped' => $wrapper,
      '#theme' => 'entity_property',
      '#property_name' => $name,
      '#access' => $property->access('view'),
      '#entity_type' => $this->entityType,
    );

    // Executive decision to only hide the body label
    if ($name == 'body_html') {
      $content[$name]['#label_hidden'] = TRUE;
    }
    $content['#attached']['css']['entity.theme'] = drupal_get_path('module', 'entity') . '/theme/entity.theme.css';
  }
}

/**
 * Class ShopifyProductMetadataController
 */
class ShopifyProductMetadataController extends EntityDefaultMetadataController {

  /**
   * Defines entity property info.
   */
  public function entityPropertyInfo() {
    $info[$this->type]['properties']['updated_at'] = array(
      'label' => t('Updated Date'),
      'type' => 'date',
      'description' => t('When the product was last updated.'),
      'schema field' => 'updated_at',
      'setter callback' => 'entity_property_verbatim_date_set',
      'getter callback' => 'entity_property_verbatim_date_get',
    );
    $info[$this->type]['properties']['created_at'] = array(
      'label' => t('Created Date'),
      'type' => 'date',
      'description' => t('When the product was created.'),
      'schema field' => 'created_at',
      'setter callback' => 'entity_property_verbatim_date_set',
      'getter callback' => 'entity_property_verbatim_date_get',
    );
    $info[$this->type]['properties']['published_at'] = array(
      'label' => t('Published Date'),
      'type' => 'date',
      'description' => t('When the product was published.'),
      'schema field' => 'published_at',
      'setter callback' => 'entity_property_verbatim_date_set',
      'getter callback' => 'entity_property_verbatim_date_get',
    );
    $info[$this->type]['properties']['product_id'] = array(
      'label' => t('Product ID'),
      'type' => 'integer',
      'description' => t('Unique product identifier from Shopify.'),
      'schema field' => 'id',
      'setter callback' => 'entity_property_verbatim_set',
      'getter callback' => 'entity_property_verbatim_get',
    );
    $info[$this->type]['properties']['variant_id'] = array(
      'label' => t('Variant ID'),
      'type' => 'integer',
      'description' => t('Unique variant identifier from Shopify.'),
      'schema field' => 'variant_id',
      'setter callback' => 'entity_property_verbatim_set',
      'getter callback' => 'entity_property_verbatim_get',
    );
    $info[$this->type]['properties']['title'] = array(
      'label' => t('Title'),
      'type' => 'text',
      'description' => t('Name of the product.'),
      'schema field' => 'title',
      'setter callback' => 'entity_property_verbatim_set',
      'getter callback' => 'entity_property_verbatim_get',
    );
    $info[$this->type]['properties']['handle'] = array(
      'label' => t('Handle'),
      'type' => 'text',
      'description' => t('Product handle from Shopify'),
      'schema field' => 'handle',
      'setter callback' => 'entity_property_verbatim_set',
      'getter callback' => 'entity_property_verbatim_get',
    );
    $info[$this->type]['properties']['product_type'] = array(
      'label' => t('Product Type'),
      'type' => 'text',
      'description' => t('Type of product.'),
      'schema field' => 'product_type',
      'setter callback' => 'entity_property_verbatim_set',
      'getter callback' => 'entity_property_verbatim_get',
    );
    $info[$this->type]['properties']['published_scope'] = array(
      'label' => t('Published Scope'),
      'type' => 'text',
      'description' => t('Product published scope from Shopify'),
      'schema field' => 'published_scope',
      'setter callback' => 'entity_property_verbatim_set',
      'getter callback' => 'entity_property_verbatim_get',
    );
    $info[$this->type]['properties']['vendor'] = array(
      'label' => t('Vendor'),
      'type' => 'text',
      'description' => t('Vendor of the product.'),
      'schema field' => 'vendor',
      'setter callback' => 'entity_property_verbatim_set',
      'getter callback' => 'entity_property_verbatim_get',
    );
    $info[$this->type]['properties']['barcode'] = array(
      'label' => t('Barcode'),
      'type' => 'text',
      'description' => t('Barcode of the product.'),
      'schema field' => 'barcode',
      'setter callback' => 'entity_property_verbatim_set',
      'getter callback' => 'entity_property_verbatim_get',
    );
    $info[$this->type]['properties']['compare_at_price'] = array(
      'label' => t('Compare at Price'),
      'type' => 'text',
      'description' => t('Compare price of the product.'),
      'schema field' => 'compare_at_price',
      'setter callback' => 'entity_property_verbatim_set',
      'getter callback' => 'entity_property_verbatim_get',
    );
    $info[$this->type]['properties']['fulfillment_service'] = array(
      'label' => t('Fulfillment Service'),
      'type' => 'text',
      'description' => t('Fulfillment service of the product.'),
      'schema field' => 'fulfillment_service',
      'setter callback' => 'entity_property_verbatim_set',
      'getter callback' => 'entity_property_verbatim_get',
    );
    $info[$this->type]['properties']['grams'] = array(
      'label' => t('Grams'),
      'type' => 'integer',
      'description' => t('Weight of the product in grams.'),
      'schema field' => 'grams',
      'setter callback' => 'entity_property_verbatim_set',
      'getter callback' => 'entity_property_verbatim_get',
    );
    $info[$this->type]['properties']['inventory_management'] = array(
      'label' => t('Inventory Management'),
      'type' => 'text',
      'description' => t('Inventory management setting for the product.'),
      'schema field' => 'inventory_management',
      'setter callback' => 'entity_property_verbatim_set',
      'getter callback' => 'entity_property_verbatim_get',
    );
    $info[$this->type]['properties']['inventory_policy'] = array(
      'label' => t('Inventory Policy'),
      'type' => 'text',
      'description' => t('Inventory policy of the product.'),
      'schema field' => 'inventory_policy',
      'setter callback' => 'entity_property_verbatim_set',
      'getter callback' => 'entity_property_verbatim_get',
    );
    $info[$this->type]['properties']['inventory_quantity'] = array(
      'label' => t('Inventory Quantity'),
      'type' => 'integer',
      'description' => t('Inventory quantity of the product.'),
      'schema field' => 'inventory_quantity',
      'setter callback' => 'entity_property_verbatim_set',
      'getter callback' => 'entity_property_verbatim_get',
    );
    $info[$this->type]['properties']['requires_shipping'] = array(
      'label' => t('Requires Shipping'),
      'type' => 'boolean',
      'description' => t('Whether the product requires shipping or not.'),
      'schema field' => 'requires_shipping',
      'setter callback' => 'entity_property_verbatim_set',
      'getter callback' => 'entity_property_verbatim_get',
    );
    $info[$this->type]['properties']['sku'] = array(
      'label' => t('SKU'),
      'type' => 'text',
      'description' => t('SKU of the product.'),
      'schema field' => 'sku',
      'setter callback' => 'entity_property_verbatim_set',
      'getter callback' => 'entity_property_verbatim_get',
    );
    $info[$this->type]['properties']['taxable'] = array(
      'label' => t('Taxable'),
      'type' => 'boolean',
      'description' => t('Whether the product is taxable or not.'),
      'schema field' => 'taxable',
      'setter callback' => 'entity_property_verbatim_set',
      'getter callback' => 'entity_property_verbatim_get',
    );
    $info[$this->type]['properties']['weight'] = array(
      'label' => t('Weight'),
      'type' => 'decimal',
      'description' => t('Weight of the product.'),
      'schema field' => 'weight',
      'setter callback' => 'entity_property_verbatim_set',
      'getter callback' => 'entity_property_verbatim_get',
    );
    $info[$this->type]['properties']['weight_unit'] = array(
      'label' => t('Weight Unit'),
      'type' => 'decimal',
      'description' => t('Name of the product.'),
      'schema field' => 'title',
      'setter callback' => 'entity_property_verbatim_set',
      'getter callback' => 'entity_property_verbatim_get',
    );
    $info[$this->type]['properties']['body_html'] = array(
      'label' => t('Body'),
      'type' => 'text',
      'description' => t('Product body HTML from Shopify.'),
      'schema field' => 'body_html',
      'setter callback' => 'entity_property_verbatim_set',
      'getter callback' => 'entity_property_verbatim_get',
    );
    // @todo price is on variants not on products, update with variant info
    $info[$this->type]['properties']['price'] = array(
      'label' => t('Price'),
      'type' => 'decimal',
      'description' => t('The product price.'),
      'schema field' => 'price',
      'setter callback' => 'entity_property_verbatim_set',
      'getter callback' => 'entity_property_verbatim_get',
    );

    return $info;
  }
}

/**
 * Class ShopifyProductsUIController
 */
class ShopifyProductsUIController extends EntityDefaultUIController {

  /**
   * Builds the entity overview form.
   */
  public function overviewForm($form, &$form_state) {
    // Only show products, not variants.
    $conditions = array('variant_id' => 0);
    $form['table'] = $this->overviewTable($conditions);
    $form['pager'] = array('#theme' => 'pager');
    return $form;
  }

  /**
   * Generates the render array for a overview table for arbitrary entities
   * matching the given conditions.
   *
   * @param $conditions
   *   An array of conditions as needed by entity_load().
   * @return Array
   *   A renderable array.
   */
  public function overviewTable($conditions = array()) {

    $query = new EntityFieldQuery();
    $query->entityCondition('entity_type', $this->entityType);

    // Add all conditions to query.
    foreach ($conditions as $key => $value) {
      $query->propertyCondition($key, $value);
    }

    if ($this->overviewPagerLimit) {
      $query->pager($this->overviewPagerLimit);
    }

    $results = $query->execute();

    $ids = isset($results[$this->entityType]) ? array_keys($results[$this->entityType]) : array();
    $entities = $ids ? entity_load($this->entityType, $ids) : array();
    ksort($entities);

    $rows = array();
    foreach ($entities as $entity) {
      $shopify_link = l(t('Edit on Shopify'), shopify_api_get_product_path($entity->product_id), array('attributes' => array('target' => '_blank')));
      $rows[] = $this->overviewTableRow($conditions, entity_id($this->entityType, $entity), $entity, array($shopify_link));
    }
    $render = array(
      '#theme' => 'table',
      '#header' => $this->overviewTableHeaders($conditions, $rows),
      '#rows' => $rows,
      '#empty' => t('None.'),
    );
    return $render;
  }

  /**
   * Generates the row for the passed entity and may be overridden in order to
   * customize the rows.
   *
   * @param $additional_cols
   *   Additional columns to be added after the entity label column.
   */
  protected function overviewTableRow($conditions, $id, $entity, $additional_cols = array()) {
    $entity_uri = entity_uri($this->entityType, $entity);

    $row[] = array(
      'data' => array(
        '#theme' => 'entity_ui_overview_item',
        '#label' => entity_label($this->entityType, $entity),
        '#name' => !empty($this->entityInfo['exportable']) ? entity_id($this->entityType, $entity) : FALSE,
        '#url' => $entity_uri ? $entity_uri : FALSE,
        '#entity_type' => $this->entityType
      ),
    );

    // Add in any passed additional cols.
    foreach ($additional_cols as $col) {
      $row[] = $col;
    }

    // Add a row for the exportable status.
    if (!empty($this->entityInfo['exportable'])) {
      $row[] = array(
        'data' => array(
          '#theme' => 'entity_status',
          '#status' => $entity->{$this->statusKey},
        )
      );
    }
    // In case this is a bundle, we add links to the field ui tabs.
    $field_ui = !empty($this->entityInfo['bundle of']) && entity_type_is_fieldable($this->entityInfo['bundle of']) && module_exists('field_ui');
    // For exportable entities we add an export link.
    $exportable = !empty($this->entityInfo['exportable']);
    // If i18n integration is enabled, add a link to the translate tab.
    $i18n = !empty($this->entityInfo['i18n controller class']);

    // Add operations depending on the status.
    if (entity_has_status($this->entityType, $entity, ENTITY_FIXED)) {
      $row[] = array(
        'data' => l(t('clone'), $this->path . '/manage/' . $id . '/clone'),
        'colspan' => $this->operationCount()
      );
    }
    else {
      $row[] = l(t('Edit Drupal Entity'), $this->path . '/manage/' . $id);

      if ($field_ui) {
        $row[] = l(t('manage fields'), $this->path . '/manage/' . $id . '/fields');
        $row[] = l(t('manage display'), $this->path . '/manage/' . $id . '/display');
      }
      if ($i18n) {
        $row[] = l(t('translate'), $this->path . '/manage/' . $id . '/translate');
      }
      if ($exportable) {
        $row[] = l(t('clone'), $this->path . '/manage/' . $id . '/clone');
      }
      if (entity_has_status($this->entityType, $entity, ENTITY_OVERRIDDEN)) {
        $row[] = l(t('revert'), $this->path . '/manage/' . $id . '/revert', array('query' => drupal_get_destination()));
      }
    }
    if ($exportable) {
      $row[] = l(t('export'), $this->path . '/manage/' . $id . '/export');
    }
    return $row;
  }

//  public function hook_menu() {
//    parent::hook_menu();
//    // @todo Add a nice page callback to point at Shopify's add product page.
  // @todo Remove the Delete callback.
  // @todo Remove the Add callback
//  }

}

/**
 * Class ShopifyProductViewsController
 */
class ShopifyProductViewsController extends EntityDefaultViewsController {

  /**
   * Defines the result for hook_views_data().
   */
  public function views_data() {
    $data = parent::views_data();

    $data['shopify_products']['variants'] = array(
      'title' => t('Variants'),
      'help' => t('Product variants.'),
      'field' => array(
        'handler' => 'views_handler_field_shopify_product_variants',
      ),
    );

    $data['shopify_products']['add_to_cart_form'] = array(
      'title' => t('Add to Cart form'),
      'help' => t('Displays an add to cart form.'),
      'field' => array(
        'handler' => 'views_handler_field_shopify_product_add_to_cart_form',
      ),
    );

    $data['shopify_products']['title'] = array(
      'title' => t('Product Title'),
      'help' => t('The product title.'),
      'field' => array(
        'field' => 'title',
        'handler' => 'views_handler_field_shopify_product',
        'click sortable' => TRUE,
        'link_to_product default' => TRUE,
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_string',
      ),
      'argument' => array(
        'handler' => 'views_handler_argument_string',
      ),
    );

    $data['shopify_products']['product_id'] = array(
      'title' => t('Product ID'),
      'help' => t('The Shopift product id associated with this product'),
      'field' => array(
        'handler' => 'views_handler_field_numeric',
        'click sortable' => TRUE,
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_numeric',
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
    );

    $data['shopify_products']['id'] = array(
      'title' => t('Entity ID'),
      'help' => t('The entity id associated with this product'),
      'field' => array(
        'handler' => 'views_handler_field_numeric',
        'click sortable' => TRUE,
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_numeric',
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
    );

    $data['shopify_products']['body_html'] = array(
      'title' => t('Body HTML'),
      'help' => t('Product description.'),
      'field' => array(
        'format' => 'filtered_html',
        'handler' => 'views_handler_field_markup',
        'click sortable' => FALSE,
      ),
    );


    $data['shopify_products']['view_link'] = array(
      'field' => array(
        'title' => t('Link'),
        'help' => t('Provide a simple link to the product.'),
        'handler' => 'views_handler_field_shopify_product_link',
      ),
    );

    $data['shopify_products']['edit_link'] = array(
      'field' => array(
        'title' => t('Edit link'),
        'help' => t('Provide a simple link to edit the product entity.'),
        'handler' => 'views_handler_field_shopify_product_edit_link',
      ),
    );

    $data['shopify_products']['price'] = array(
      'title' => t('Starting At Price'),
      'help' => t('The lowest price for a given variant & product combination'),
      'field' => array(
        'handler' => 'views_handler_field_shopify_price',
        'click sortable' => TRUE,
      ),
    );

    return $data;
  }

}
