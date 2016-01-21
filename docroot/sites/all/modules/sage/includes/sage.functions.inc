<?php

/**
 * @file
 * Contains main API and helper functions.
 */

// Include function.inc and sage.search.inc files.
module_load_include('inc', 'sage', 'includes/sage.xmlstrtoarray');

/**
 * POST request pattern for SAGE API.
 *
 * @param string $xmlrequest
 *   The structured xml string to be posted.
 * @param string $url
 *   The SAGE XML API url to send the request.
 *
 * @return string|false
 *   If there are results it returns an xml structured string.
 *   Otherwise it returns FALSE.
 */
function sage_http_post_request($xmlrequest, $url = "") {

  $url = variable_get('sage_api_auth_url');

  // Add xml wrapper tag.
  $xmlrequest = "<XMLDataStreamRequest>" . $xmlrequest . "</XMLDataStreamRequest>";

  $response = drupal_http_request(
      $url, array(
        'headers' => array('Content-Type' => 'text/xml; charset=UTF-8'),
        'data' => $xmlrequest,
        'method' => 'POST',
        'timeout' => 10,
      )
    );

  // Check if we have arguments.
  if ($url == "" || $xmlrequest == "") {
    return FALSE;
  }

  // Get xml data.
  if (empty($response->error)) {
    $result = $response->data;
    return $result;
  }
  else {
    // Show the response if we have enabled debug.
    if (variable_get('sage_api_debug', "") == 1 && module_exists('devel')) {
      dpm($response);
    }
    // Also create a watchdog entry for debugging.
    if (variable_get('sage_api_debug', "") == 1 && module_exists('watchdog')) {
      watchdog('sage', '%response', array('%response' => $response), WATCHDOG_INFO);
    }
    return FALSE;
  }
}

/**
 * Generate the xml code needed to authenticate SAGE post requests.
 *
 * @param string $acctid
 *   The SAGE Member action ID.
 * @param string $loginid
 *   The SAGE Member Login ID.
 * @param string $password
 *   The SAGE Member password.
 *
 * @return string
 *   Xml structured string.
 */
function sage_auth_xml($acctid = "", $loginid = "", $password = "") {

  // SAGE credentials.
  $acctid = variable_get('sage_api_auth_acctid');
  $loginid = variable_get('sage_api_auth_loginid');
  $password = variable_get('sage_api_auth_pass');
  $version = "3.2";

  // Xml used to build the request.
  $xml = "";
  $xml .= "<Ver>" . $version . "</Ver>";
  $xml .= "<Auth>";
  $xml .= "<AcctID>" . $acctid . "</AcctID>";
  $xml .= "<LoginID>" . $loginid . "</LoginID>";
  $xml .= "<Password>" . $password . "</Password>";
  $xml .= "</Auth>";

  return $xml;
}

/**
 * Sort options for Search results.
 *
 * @return array
 *   A simple array with sort options.
 */
function sage_sort_options() {

  return array(
    'DEFAULT' => t('Default'),
    'PRICE' => t('Price (Low - High)'),
    'PRICEHIGHLOW' => t('Price (High - Low)'),
    'BESTMATCH' => t('Best Match'),
    'POPULARITY' => t('Most Popular'),
    // 'PREFGROUP' => t('Sort by preference groups.'),
  );
}

/**
 * Items per page options for Search results.
 *
 * @return array
 *   A simple array with items per page options.
 */
function sage_items_per_page_options() {

  return array(
    '10' => t('10'),
    '25' => t('25'),
    '50' => t('50'),
    '100' => t('100'),
  );
}

/**
 * Default search options.
 *
 * @ToDo: Create more default settings as variables.
 *
 * @return array
 *   A simple array with the default search options.
 *   These options will be overriden within functions accordingly.
 */
function sage_default_search_option() {

  return array(
    'category' => '',
    'keywords' => '',
    'colors' => '',
    'themes' => '',
    'spc' => '',
    'lop' => '',
    // 'pricehigh' => '',
    // 'pricelow' => '',.
    'hip' => '',
    'quicksearch' => '',
    'qty' => '',
    'madeinusa' => '',
    'envfriendly' => '',
    'includerush' => '',
    'newproduct' => '',
    'prefgroups' => '',
    // User UI options.
    'sort' => variable_get('sage_api_display_sorting', ""),
    'startnum' => 1,
    'maxtotalitems' => variable_get('sage_api_display_maxtotalitems', 1000),
    'maxrecs' => variable_get('sage_api_display_maxrecs', 15),
  );
}

/**
 * Generate the xml code needed to get SAGE (Advanced) Search results.
 *
 * @param array $option
 *   The options to pass to the SAGE XML API for Search.
 *
 * @return string
 *   Structured xml string.
 */
function sage_search_xml(array $option) {

  $default_options = sage_default_search_option();

  // Merge $options with the default options.
  $option = array_merge($default_options, $option);

  // Xml used to get the request.
  $xml = "";
  $xml .= "<Search>";

  // $xml .= "<Category>CATEGORY NAME(S) OR NUMBER(S)</Category>";
  $xml .= "<Category>" . sage_beutify_string($option['category']) . "</Category>";

  // $xml .= "<Keywords>KEYWORDS</Keywords>";
  $xml .= "<Keywords>" . sage_beutify_string($option['keywords']) . "</Keywords>";
  $xml .= "<Themes>" . $option['themes'] . "</Themes>";

  // $xml .= "<Colors>COLORS</Colors>";
  $xml .= "<Colors>" . sage_beutify_string($option['colors']) . "</Colors>";

  // $xml .= "<QuickSearch>QUICKSEARCH INPUT</QuickSearch>";
  $xml .= "<QuickSearch>" . sage_beutify_string($option['quicksearch']) . "</QuickSearch>";

  // $xml.="<ItemNum>PRODUCT’S ACTUAL ITEM NUMBER</ItemNum>";
  // $xml.="<ItemNum>".sage_beutify_string($option['itemnumber'])."</ItemNum>";

  // $xml .= "<SPC>SAGE PRODUCT CODE or SAGE ID</SPC>";
  $xml .= "<SPC>" . $option['spc'] . "</SPC>";

  // $xml .= "<PriceLow>LOWEST PRICE IN USD (e.g., “0.50”)</PriceLow>";
  $xml .= "<PriceLow>" . sage_beutify_string($option['lop']) . "</PriceLow>";

  // $xml .= "<PriceHigh>HIGHEST PRICE IN USD (e.g., “2.00”)</PriceHigh>";
  $xml .= "<PriceHigh>" . sage_beutify_string($option['hip']) . "</PriceHigh>";

  // $xml .= "<Qty>QUANTITY</Qty>";
  $xml .= "<Qty>" . $option['qty'] . "</Qty>";

  // $xml .= "<MadeIn>Blank for all or two-digit country code</MadeIn>";
  $xml .= "<MadeIn>" . sage_beutify_string($option['madeinusa']) . "</MadeIn>";

  // $xml.="<EnvFriendly>0 or 1 (1=Env. Friendly Products Only)</EnvFriendly>";
  $xml .= "<EnvFriendly>" . $option['envfriendly'] . "</EnvFriendly>";

  // $xml .= "<Sort>SEE BELOW FOR SORT OPTIONS</Sort>";
  $xml .= "<Sort>" . $option['sort'] . "</Sort>";

  // $xml .= "<StartNum>RECORD NUMBER TO START WITH (1=FIRST)</StartNum>";
  $xml .= "<StartNum>" . $option['startnum'] . "</StartNum>";

  // $xml .= "<MaxRecs>integer (MAX RECORDS TO RETURN PER PAGE)</MaxRecs>";
  $xml .= "<MaxRecs>" . $option['maxrecs'] . "</MaxRecs>";

  // $xml .= "<MaxTotalItems>MAX ITEMS TO FIND (<=1000)</MaxTotalItems>";
  $xml .= "<MaxTotalItems>" . $option['maxtotalitems'] . "</MaxTotalItems>";

  // $xml .= "<Verified>0 or 1 (1=Verified Products Only)</Verified>";
  $xml .= "<Verified>1</Verified>";

  // $xml .= "<IncludeRush>Y OR N</IncludeRush>";
  $xml .= "<IncludeRush>" . $option['includerush'] . "</IncludeRush>";

  // $xml .= "<NewProduct>0 or 1 (1=New Products Only)</NewProduct>";
  $xml .= "<NewProduct>" . $option['newproduct'] . "</NewProduct>";

  // Other parameters that are not used at the moment.
  // $xml .= "<ItemName>PRODUCT’S ITEM NAME</ItemName>";
  // $xml .= "<Recyclable>0 or 1 (1=Recyclable Products Only)</Recyclable>";
  // $xml .= "<UnionShop>0 or 1 (1=Union Shop Only)</UnionShop>";
  // $xml .= "<AllAudiences>0 or 1 (1=All Audiences Only)</AllAudiences>";
  // $xml .= "<ProdTime>MIN PROD TIME IN DAYS OR “0” FOR ANY</ProdTime>";
  // $xml .= "<LineName>SPECIFIC SUPPLIER’S LINE NAME</LineName>";
  // $xml .= "<SiteCountry>Site country code (if diff from acct)</SiteCountry>";
  // $xml .= "<ExtraReturnFields>SEE BELOW</ExtraReturnFields>";

  // $xml .= "<PrefGroups>BLANK=ALL OR COMMA-SEPARATED LIST OF #S</PrefGroups>";
  $xml .= "<PrefGroups>" . sage_beutify_string($option['prefgroups']) . "</PrefGroups>";

  $xml .= "</Search>";

  return $xml;
}

/**
 * Generate the xml code needed to get SAGE CategoryList.
 *
 * @param string $sort
 *   Sort options to pass to the SAGE XML API for CategoryList.
 *
 * @return string
 *   Structured xml string.
 */
function sage_categorylist_xml($sort = "CATNAME") {

  // Available options
  // $sort = "NAME";
  // $sort = "CATNAME";
  // Xml used to get the request.
  $xml = "";
  $xml .= "<CategoryList>";
  $xml .= "<GetList>1</GetList>";
  $xml .= "<Sort>" . $sort . "</Sort>";
  $xml .= "</CategoryList>";

  return $xml;
}

/**
 * Generate the xml code needed to get SAGE ThemeList.
 *
 * @return string
 *   Structured xml string.
 */
function sage_themelist_xml() {

  // Xml used to build the request.
  $xml = "";
  $xml .= "<ThemeList>";
  $xml .= "<GetList>1</GetList>";
  $xml .= "</ThemeList>";

  return $xml;
}

/**
 * Generate the xml code needed to get SAGE ProductDetail.
 *
 * @param string $product_id
 *   The SAGE product ID.
 *
 * @return string
 *   Structured xml string.
 */
function sage_productdetail_xml($product_id) {

  // Xml used to get the request.
  $xml = "";
  $xml .= "<ProductDetail>";
  $xml .= "<ProductID>" . $product_id . "</ProductID>";
  $xml .= "</ProductDetail>";

  return $xml;
}

/**
 * Generate the xml code needed to get SAGE SupplierInfo from SAGE supplier ID.
 *
 * @param string $supplier_id
 *   The SAGE XML API Supplier ID.
 * @param string $general_info
 *   Option to include GENINFO on results.
 *
 * @return string
 *   Structured xml string.
 */
function sage_supplierinfo_xml($supplier_id, $general_info = "") {

  // Available options
  // $general_info = "GENINFO";
  // Xml used to get the request.
  $xml = "";
  $xml .= "<SupplierInfo>";
  $xml .= "<SuppID>" . $supplier_id . "</SuppID>";
  $xml .= "<ExtraReturnFields>" . $general_info . "</ExtraReturnFields>";
  $xml .= "</SupplierInfo>";

  return $xml;
}

/**
 * Generate the xml code needed to get SAGE SupplierList.
 *
 * @param string $sort
 *   The sort options for SupplierList.
 *   Available options are: SAGEID, COMPANY, LINE.
 * @param string $alllines
 *   Option to include All LINES on results.
 *   If "$alllines = 1" then it returns all ALL LINE NAMES.
 *
 * @return string
 *   Structured xml string.
 */
function sage_supplierlist_xml($sort = "SAGEID", $alllines = 0) {

  // Xml used to get the request.
  $xml = "";
  $xml .= "<SupplierList>";
  $xml .= "<GetList>1</GetList>";
  $xml .= "<AllLines>" . $alllines . "</AllLines>";
  $xml .= "<Sort>" . $sort . "</Sort>";
  $xml .= "</SupplierList>";

  return $xml;
}

/**
 * Generic function that builds the SAGE XML API post request.
 *
 * @param string $current_xml
 *   Structured xml string.
 *
 * @return array
 *   Php array with the results.
 */
function sage_post_request_builder($current_xml) {

  // Get current user authentication xml.
  $auth_xml = sage_auth_xml();

  // DataStream final xml to post.
  $xmlrequest = $auth_xml . $current_xml;

  // POST request.
  $post = sage_http_post_request($xmlrequest);

  // Get php array from request results.
  $results = sage_xmlstr_to_array($post);

  return $results;
}

/**
 * Get php array from API request for CategoryList.
 *
 * @param string $sort
 *   Sort options for the CategoryList.
 *
 * @return array
 *   Php array with the results.
 */
function sage_post_datastream_categorylist($sort = "NAME") {

  // Current xml to post.
  $current_xml = sage_categorylist_xml($sort);
  return sage_post_request_builder($current_xml);
}

/**
 * Get php array from API request for ThemeList.
 *
 * @return array
 *   Php array with the results.
 */
function sage_post_datastream_themelist() {

  // Current xml to post.
  $current_xml = sage_themelist_xml();
  return sage_post_request_builder($current_xml);
}

/**
 * Get php array from API request for SupplierList.
 *
 * @param string $sort
 *   Sort options for the SupplierList.
 * @param string $alllines
 *   Option to include ALL LINES on results.
 *
 * @return array
 *   Php array with the results.
 */
function sage_post_datastream_supplierlist($sort = "SAGEID", $alllines = "") {

  // Current xml to post.
  $current_xml = sage_supplierlist_xml($sort, $alllines);
  return sage_post_request_builder($current_xml);
}

/**
 * Get php array from API request for SupplierInfo.
 *
 * @param string $supplier_id
 *   The SAGE XML API Supplier ID.
 * @param string $general_info
 *   Option to include GENINFO on results.
 *
 * @return array
 *   Php array with the results.
 */
function sage_post_datastream_supplierinfo($supplier_id, $general_info = "") {

  // Current xml to post.
  $current_xml = sage_supplierinfo_xml($supplier_id, $general_info);
  return sage_post_request_builder($current_xml);
}

/**
 * Get php array from API request for ProductDetail.
 *
 * @param string $product_id
 *   The SAGE XML API Product ID for ProductDetail.
 *
 * @return array
 *   Php array with the results.
 */
function sage_post_datastream_productdetail($product_id) {

  // $general_info = "GENINFO";
  // Current xml to post.
  $current_xml = sage_productdetail_xml($product_id);
  return sage_post_request_builder($current_xml);
}

/**
 * Get php array from API request for Search.
 *
 * @param array $option
 *   The option to pass to the SAGE XML API for Search.
 *
 * @return array
 *   Php array with the results.
 */
function sage_post_datastream_search(array $option) {

  // Current xml to post.
  $current_xml = sage_search_xml($option);

  // Convert xml to php.
  return sage_post_request_builder($current_xml);
}

/**
 * Get all SAGE Categories as php array.
 *
 * @return array
 *   Php array with the Catgories sorted by Name.
 *   Useful for select list or menus.
 */
function sage_get_categories_array() {

  // Get categories from SAGE API.
  $categories = sage_post_datastream_categorylist();

  asort($categories);

  // Initialize array with the default only value.
  $categories_array = array();
  $categories_array[] = t('-- All Categories --');

  // Get Num and Name of each category from SAGE XML API.
  foreach ($categories['CategoryList']['Category'] as $category) {
    $num = $category['Num'];
    $name = $category['Name'];

    $categories_array[$num] = $name;
  }
  // Sort by Category Name.
  asort($categories_array);

  return $categories_array;
}

/**
 * Get all SAGE Themes list as php array.
 *
 * @return array
 *   Php array with all the Themes.
 */
function sage_get_themelist_array() {

  // Get Themes from SAGE API.
  $post = sage_post_datastream_themelist();
  return $post['ThemeList']['Theme'];
}

/**
 * Get all SAGE Suppliers list as php array.
 *
 * @return array
 *   Php array with the Suppliers.
 */
function sage_get_supplierlist_array() {

  // Get Suppliers from SAGE API.
  $post = sage_post_datastream_supplielist();
  return $post['SupplierList']['Supplier'];
}

/**
 * Load SAGE Search products.
 *
 * @param array $option
 *   The array of options to pass to the SAGE XML API for Search.
 *
 * @return array
 *   Php array with the results.
 */
function sage_load_search(array $option) {

  // Get xml post request results.
  $post = sage_post_datastream_search($option);

  return $post;
}

/**
 * Display Category products in list.
 *
 * Each Products has Name, Image, Price and (Drupal) internal path.
 * Check sage_load_search() above for more details.
 *
 * @param array $post
 *   The xml to POST to the SAGE XML API for Search.
 *
 * @return string|false
 *   If there are results return a valid markup (string) that will be ready to
 *   get rendered.
 *   Otherwise return FALSE.
 */
function sage_display_search_results(array $post) {

  // Get only Category products.
  if (isset($post['SearchResults']['Items']['Item'])) {
    $products = $post['SearchResults']['Items']['Item'];

    // Loop to get each product.
    $markup = "";
    foreach ($products as $key => $value) {
      $product_id = $products[$key]['ProductID'];
      $product_name = sage_clean_text($products[$key]['PrName']);
      $product_path = sage_clean_path($product_id, $product_name);
      $image_path = sage_strip_path($products[$key]['ThumbPicLink']);
      $product_image = str_replace("RS=150", "RS=300", $image_path);

      $markup .= "<article class='product-item'>";
      $markup .= "<a href='" . $product_path . "'>";
      $markup .= "<div class='product-image'>";
      $markup .= "<img src='" . $product_image . "' alt='Product Image'/>";
      $markup .= "</div>";
      $markup .= "<div class='product-title'>" . $product_name . "</div>";

      if (!empty($products[$key]['Prc'])) {
        $product_price = $products[$key]['Prc'];
        $markup .= "<div class='product-price'>$" . $product_price . "</div>";
      }

      $markup .= "</a>";
      $markup .= "</article>";
    }
    return $markup;
  }
  else {
    return FALSE;
  }
}

/**
 * Get all Product attributes from SAGE API in array.
 *
 * @param int $product_id
 *   The SAGE XML API Product ID.
 *
 * @return array
 *   Php array with the Product details.
 */
function sage_load_product($product_id) {

  $post = sage_post_datastream_productdetail($product_id);
  $product = $post['ProductDetail'];

  return $product;
}

/**
 * Display a SAGE Product markup from the Product php object.
 *
 * @param object $product
 *   The Product php object returned SAGE XML API.
 *
 * @return string
 *   A valid markup (string) with the Product details.
 */
function sage_display_product($product) {

  $product_id = $product['ProductID'];

  $product_name = $product['PrName'];

  $product_category = $product['Category'];

  $product_image = $product['PicLink'];

  $product_description = $product['Description'];

  $product_spc = $product['SPC'];
  // SAGE code.
  $product_imprint = array(
    'Imprint Area' => $product['ImprintArea'],
    'Second Imprint Area' => $product['SecondImprintArea'],
    'Decoration method' => $product['DecorationMethod'],
    'Price includes' => $product['PriceIncludes'],
  );
  $product_colors = $product['Colors'];

  $product_dimensions = $product['Dimensions'];

  $product_themes = $product['Themes'];

  $product_setup = array(
    'Setup:' => $product['SetupChg'],
    'Repeat Setup:' => $product['RepeatSetupChg'],
    'Screen Setup:' => $product['ScreenChg'],
    'Plate Setup:' => $product['PlateChg'],
    'Die Setup:' => $product['DieChg'],
    'Additional Color Setup:' => $product['AddClrChg'],
  );

  $product_quantity_prices = array(
    $product['Qty1'] => $product['Prc1'],
    $product['Qty2'] => $product['Prc2'],
    $product['Qty3'] => $product['Prc3'],
    $product['Qty4'] => $product['Prc4'],
    $product['Qty5'] => $product['Prc5'],
    $product['Qty6'] => $product['Prc6'],
  );

  $product_color_running_charge = array(
    'AddClrRunChg1' => $product['AddClrRunChg1'],
    'AddClrRunChg2' => $product['AddClrRunChg2'],
    'AddClrRunChg3' => $product['AddClrRunChg3'],
    'AddClrRunChg4' => $product['AddClrRunChg4'],
    'AddClrRunChg5' => $product['AddClrRunChg5'],
    'AddClrRunChg6' => $product['AddClrRunChg6'],
  );

  $product_pieces_per_quantity = array(
    'PiecesPerUnit1' => $product['PiecesPerUnit1'],
    'PiecesPerUnit2' => $product['PiecesPerUnit2'],
    'PiecesPerUnit3' => $product['PiecesPerUnit3'],
    'PiecesPerUnit4' => $product['PiecesPerUnit4'],
    'PiecesPerUnit5' => $product['PiecesPerUnit5'],
    'PiecesPerUnit6' => $product['PiecesPerUnit6'],
  );

  $quantity_keys = array_keys($product_quantity_prices);

  $color_charges_values = array_values($product_color_running_charge);

  // Product Color Running Charge Quantity.
  $product_crchq = sage_special_combine($quantity_keys, $color_charges_values);

  $product_packaging = array(
    'Package' => $product['Package'],
    'Weight per Carton' => $product['WeightPerCarton'],
    'Units per Carton' => $product['UnitsPerCarton'],
    'Carton length' => $product['CartonL'] . '"',
    'Carton width' => $product['CartonW'] . '"',
    'Carton height' => $product['CartonH'] . '"',
  );

  $product_production_time = $product['ProdTime'];
  $product_comment = $product['Comment'];

  // Build markup.
  $markup = "";

  // Top details image + description etc.
  $markup .= "<div class='details-top clearfix'>";

  $markup .= "<div class='details-top-left'>";
  $markup .= "<img src=" . $product_image . " />";
  $markup .= "</div><!-- /.details-top-left -->";

  $markup .= "<div class='details-top-right'>";

  $markup .= "<h2>" . sage_clean_text($product_name) . "</h2>";

  $markup .= "<div class='product-spc'>";
  $markup .= "<label>Item #</label>" . $product_spc;
  $markup .= "</div>";

  $markup .= "<div class='product-description'>";
  $markup .= sage_clean_text($product_description);
  $markup .= "</div>";

  $markup .= "</div><!-- /.details-top-right -->";

  $markup .= "</div><!-- /.details-top -->";

  // Product quantity.
  $markup .= "<div class='product-quantity'>";
  $markup .= "<div class='quantity-item quantity-item-labels'>";
  $markup .= "<div class='quantity-qty'>Quantity</div>";
  $markup .= "<div class='quantity-price'>Your Price (each)</div>";
  $markup .= "</div>";

  // Create a table like list of pari values.
  foreach ($product_quantity_prices as $quantity => $price) {
    if ($quantity != 0) {
      $markup .= "<div class='quantity-item'>";
      $markup .= "<div class='quantity-qty'>" . $quantity . "</div>";
      $markup .= "<div class='quantity-price'>$" . $price . "</div>";
      $markup .= "</div>";
    }
  }
  $markup .= "</div>";

  // Setup charges etc.
  $markup .= "<div class='product-setup-charges'>";
  foreach ($product_setup as $key => $value) {
    if ($value != 0 && $value != NULL) {
      $markup .= $key . " $" . $value . ", ";
    }
  }
  if ($product_color_running_charge) {
    $markup .= "Additional Color Running Charge: ";
  }
  foreach ($product_crchq as $key => $value) {
    if ($value != 0 && $value != NULL) {
      $markup .= $key . " - $" . $value . ", ";
    }
  }
  $markup .= "</div>";

  // Colors.
  $markup .= "<div class='product-colors toggle-field'>";
  $markup .= "<div class='toggle-header'>Colors</div>";
  $markup .= "<div class='toggle-content'>" . $product_colors . "</div>";
  $markup .= "</div>";

  // Imprint.
  $markup .= "<div class='product-imprint toggle-field'>";
  $markup .= "<div class='toggle-header'>Imprint</div>";
  $markup .= "<div class='toggle-content'>";
  foreach ($product_imprint as $key => $value) {
    if ($value != 0 && $value != NULL) {
      $markup .= "<p>" . $key . ": " . $value . "</p>";
    }
  }
  $markup .= "</div>";
  $markup .= "</div>";

  // Packaging.
  $markup .= "<div class='product-packaging toggle-field'>";
  $markup .= "<div class='toggle-header'>Packaging</div>";
  $markup .= "<div class='toggle-content'>";
  foreach ($product_packaging as $key => $value) {
    if ($value != 0 && $value != NULL) {
      $markup .= "<p>" . $key . ": " . $value . "</p>";
    }
  }
  $markup .= "</div>";
  $markup .= "</div>";

  // Production time.
  $markup .= "<div class='product-production-time toggle-field'>";
  $markup .= "<div class='toggle-header'>Production Time</div>";
  $markup .= "<div class='toggle-content'>" . $product_production_time . "</div>";
  $markup .= "</div>";

  // Request Quote.
  $markup .= '<div class="product-buttons product-request-order">';
  $markup .= l(t('Request Quote'), 'sage/quote/product/' . $product_id);
  $markup .= "</div>";
  // Order.
  $markup .= '<div class="product-buttons product-order">';
  $markup .= l(t('Order'), 'sage/order/product/' . $product_id);
  $markup .= "</div>";

  return $markup;
}

/**
 * Get Product name from object.
 *
 * @param object $product
 *   Product as php object.
 *
 * @return string
 *   Clean string.
 */
function sage_product_title($product) {

  if (isset($product['PrName'])) {
    return sage_clean_text($product['PrName']);
  }
}

/**
 * Get Category name from category_id.
 *
 * @param string $arg
 *   The category id.
 *
 * @return string
 *   The category name.
 */
function sage_category_title($arg) {

  $categories = sage_get_categories_array();
  if (isset($categories[$arg])) {
    return sage_clean_text($categories[$arg]);
  }
}

/**
 * Get pager and related data from SAGE API post request results.
 *
 * @param array $post
 *   The php results from a SAGE XML API post request.
 *
 * @return array
 *   The pager for current results.
 */
function _sage_search_pager(array $post) {

  // Total results found.
  $default_options = sage_default_search_option();
  $total_items = $post['SearchResults']['TotalFound'];

  if (!isset($post['SearchResults']['Items']['Item'])) {
    $pager = array();
    return $pager;
  }
  // Products.
  $products = $post['SearchResults']['Items']['Item'];

  // Get current values from url parameters or from post request results.
  $parameters = drupal_get_query_parameters();
  // Maxrecs.
  if (isset($parameters['maxrecs'])) {
    $maxrecs = $parameters['maxrecs'];
  }
  else {
    $maxrecs = $default_options['maxrecs'];
  }

  // Startnum.
  if (isset($parameters['startnum'])) {
    $startnum = $parameters['startnum'];
  }
  else {
    $startnum = $default_options['startnum'];
  }

  // Get last product Count attribute.
  $endnum = end($products)['Count'];

  // Total pages.
  $total_pages = ceil($total_items / $maxrecs);

  // Current page.
  $current_page = ceil($startnum / $maxrecs);

  // Previous page first count.
  if ($startnum > $maxrecs) {
    $prev_startnum = $startnum - $maxrecs;
  }
  else {
    $prev_startnum = NULL;
  }

  // Next page first count.
  if ($current_page < $total_pages) {
    $next_startnum = $endnum + 1;
  }

  // Build pager.
  $pager = array(
    'current_path' => current_path(),
    'current_startnum' => $startnum,
    'current_endnum' => $endnum,
    'prev_startnum' => $prev_startnum,
    'next_startnum' => $next_startnum,
    'current_page' => $current_page,
    'total_pages' => $total_pages,
    'total_items' => $total_items,
  );

  return $pager;
}

/**
 * Renders the pager. Check _sage_search_pager() for more details.
 *
 * @param array $pager
 *   The pager as array.
 *
 * @return string
 *   A valid markup ready to be used on templates.
 */
function _sage_display_pager_option(array $pager) {

  if (empty($pager)) {
    return "";
  }
  // Get current path.
  $current_path = current_path();

  // Get url parameters.
  $parameters = drupal_get_query_parameters();

  // Build markup.
  $markup = "<div class='pager-pages'>";
  $markup .= "<label>Page: </label>";
  $markup .= "<div class='current-page'>" . $pager['current_page'] . "</div>";
  $markup .= "<div class='pager-separetor'> of </div>";
  $markup .= "<div class='total-page'>" . $pager['total_pages'] . "</div>";

  // Previous page.
  if ($pager['prev_startnum']) {
    unset($parameters['startnum']);
    $parameters['startnum'] = $pager['prev_startnum'];
    $link = l(t("Prev"), $current_path, array('query' => $parameters));
    $markup .= "<div class='prev-page'>" . $link . "</div>";
  }

  // Next page.
  if ($pager['next_startnum']) {
    unset($parameters['startnum']);
    $parameters['startnum'] = $pager['next_startnum'];
    $link = l(t("Next"), $current_path, array('query' => $parameters));
    $markup .= "<div class='next-page'>" . $link . "</div>";
  }

  $markup .= "</div><!-- /.pager-pages -->";

  // Items of the page.
  $markup .= "<div class='pager-items'>";
  $markup .= "<label>Showing items: </label>";
  $markup .= "<div class='current-startnum'>" . $pager['current_startnum'] . "</div>";
  $markup .= "<div class='pager-separetor'> to </div>";
  $markup .= "<div class='current-endnum'>" . $pager['current_endnum'] . "</div>";
  $markup .= "<div class='pager-separetor'> of </div>";
  $markup .= "<div class='total-items'>" . $pager['total_items'] . "</div>";
  $markup .= "</div><!-- /.pager-items -->";

  return $markup;
}

/**
 * Create a links list with sorting options.
 *
 * @return string
 *   Creates a list of urls that change path sort parameters.
 */
function _sage_display_sort_option() {

  $parameters = drupal_get_query_parameters();
  unset($parameters['sort']);

  $items = "";
  foreach (sage_sort_options() as $key => $value) {
    $parameters['sort'] = $key;
    $items .= "<li>" . l($value, current_path(), array('query' => $parameters)) . "</li>";
  }

  return $items;
}

/**
 * Create a links list with items per page options.
 *
 * @return string
 *   Creates a list of urls that change path items per page parameters.
 */
function _sage_display_items_per_page_option() {

  $parameters = drupal_get_query_parameters();
  unset($parameters['maxrecs']);

  $items = "<li class='list-title'>" . t('Per page') . ": </li>";
  foreach (sage_items_per_page_options() as $key => $value) {
    $parameters['maxrecs'] = $key;
    $items .= "<li>";
    $items .= l($value, current_path(), array('query' => $parameters));
    $items .= "</li>";
  }

  return $items;
}

/**
 * SAGE reports for Categories and Themes.
 *
 * @param string $type
 *   The type to get the reports.
 *
 * @return array
 *   Php array with results to be used on theme_table function.
 */
function sage_reports_results($type = 'categories') {

  if ($type == 'categories') {
    $categories = sage_get_categories_array();
    // Remove first value sinc it is the "-- All Categories --" helper.
    unset($categories['0']);
    // Table header.
    $header = array(
      array('data' => t('a/a')),
      array('data' => t('Category')),
      array('data' => t('SAGE ID')),
      array('data' => t('Products')),
      array('data' => t('View')),
    );
    $rows = array();
    $index = 1;

    foreach ($categories as $id => $name) {
      $option = array(
        'category' => $id,
        'maxrecs' => 1,
      );
      $total_found = 0;
      $link = "<a href=/sage/category/" . $id . "/" . $name . ">View</a>";
      // $post = sage_load_search($option);
      // if ($post['SearchResults']['TotalFound']) {
      // $total_found = $post['SearchResults']['TotalFound'];
      // }

      // Table rows.
      $rows[$id]['internal'] = $index++;
      $rows[$id]['category_name'] = $name;
      $rows[$id]['sage_id'] = $id;
      $rows[$id]['count_products'] = $total_found;
      $rows[$id]['category_view'] = $link;
    }
    $results['header'] = $header;
    $results['rows'] = $rows;
    return $results;
  }
  if ($type == 'themes') {
    $themes = sage_get_themelist_array();
    // Table header.
    $header = array(
      array('data' => t('a/a')),
      array('data' => t('Theme Name')),
      array('data' => t('Theme ID')),
      array('data' => t('Products')),
      array('data' => t('View')),
    );
    $rows = array();
    $index = 1;

    foreach ($themes as $id => $name) {
      $option = array(
        'themes' => $name,
        'maxrecs' => 1,
      );
      $total_found = 0;
      $link = "<a href=/sage/search/results/?themes=" . $name . ">View</a>";
      // $post = sage_load_search($option);
      // if ($post['SearchResults']['TotalFound']) {
      // $total_found = $post['SearchResults']['TotalFound'];
      // }

      // Table rows.
      $rows[$id]['internal'] = $index++;
      $rows[$id]['theme_name'] = $name;
      $rows[$id]['theme_id'] = '';
      $rows[$id]['count_products'] = $total_found;
      $rows[$id]['theme_view'] = $link;
    }
    $results['header'] = $header;
    $results['rows'] = $rows;
    return $results;
  }
}

/**
 * Get a query value for a url parameter.
 *
 * If url is not set then the default $_GET is used.
 *
 * @param string $url
 *   The page url.
 * @param string $key
 *   A url parameter key.
 *
 * @return string|array
 *   String if we want a key value or Array (all the values) if we give no key.
 */
function sage_get_url_parameter($url = "", $key = "") {

  $parameters = array();

  if (drupal_get_query_parameters() != "" && $url == "") {
    $parameters = drupal_get_query_parameters();
  }
  if ($url != "") {
    // Get query string from url.
    $query = parse_url($url, PHP_URL_QUERY);
    // Get url parameters in variable $parameters.
    parse_str($url, $parameters);
  }
  if (isset($parameters[$key]) && !empty($parameters[$key])) {
    return filter_xss($parameters[$key]);
  }
  return $parameters;
}

/**
 * Processing string that contains query parameters etc.
 *
 * @param string $string
 *   A url as plain string.
 *
 * @return string
 *   A formatted string.
 */
function sage_beutify_string($string) {

  if (isset($string) && !empty($string)) {
    return drupal_strtolower(preg_replace('/\s+/', ' ', $string));
  }
}

/**
 * Combine 2 arrays that may be of different length.
 *
 * @param array $arr1
 *   First array.
 * @param array $arr2
 *   Seccond array.
 *
 * @return array
 *   Combined array.
 */
function sage_special_combine(array $arr1, array $arr2) {

  $count = min(count($arr1), count($arr2));
  $first_values = array_slice($arr1, 0, $count);
  $second_values = array_slice($arr2, 0, $count);
  return array_combine($first_values, $second_values);
}

/**
 * Remove values of zero.
 *
 * @param string $value
 *   String to be formatted.
 *
 * @return string|false
 *   Returns the formatted string or FALSE.
 */
function sage_boolean_zero($value) {

  if ($value != '0') {
    return $value;
  }
  else {
    return FALSE;
  }
}

/**
 * Returns clean url for a path string using pathauto module.
 *
 * @param int $id
 *   An integer.
 * @param string $name
 *   A string.
 * @param string $path
 *   A string (internal path). IMPORTANT: do not add trailing slash!
 *
 * @return string
 *   An internal path formatted with pathauto.
 */
function sage_clean_path($id, $name, $path = '/sage/product') {

  $name_clean = str_replace(" ", "-", $name);
  if (module_exists('pathauto')) {
    module_load_include('inc', 'pathauto', 'pathauto');
    $name_clean = pathauto_cleanstring($name_clean);
  }
  return $path . "/" . $id . "/" . $name_clean;
}

/**
 * Remove "&" from url.
 *
 * @param string $path
 *   The path to be formatted.
 *
 * @return string
 *   The formatted path.
 */
function sage_strip_path($path) {

  if ($path != "") {
    return str_replace("&", "&amp;", $path);
  }
}

/**
 * Replace special - strange characters that come from XML.
 *
 * @param string $text
 *   The string to be formatted.
 *
 * @return string
 *   The formatted string.
 */
function sage_clean_text($text) {

  $replace = array(
    'Â' => " ",
    'â„¢' => "™",
  );
  $clean_text = str_replace(array_keys($replace), array_values($replace), $text);

  return $clean_text;
}
