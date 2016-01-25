<?php

/**
 * @file
 * The HTML Tidy hook implementations for Drupal's Filter system.
 */

/**
 * Implementation of hook_filter_info().
 */
function htmltidy_filter_info() {

  $filters['htmltidy'] = array(
    'title' => t('HTML Tidy'),
    'description' => t('Corrects faulty and invalid HTML according to htmltidy configuration rules.'),
    'process callback' => 'htmltidy_filter_htmltidy_process',
    'settings callback' => 'htmltidy_filter_htmltidy_settings',
    'tips callback'  => 'htmltidy_filter_htmltidy_tips',
  'weight' => -10,
  );
  return $filters;
}

/**
 * Implementation of hook_filter_FILTER_process().
 */
function htmltidy_filter_htmltidy_process($text, $filter, $format, $langcode, $cache, $cache_id) {
  global $_htmltidy_filter;
  $errors = array();
  $warnings = array();

  // Grab the filter settings so we can use them.
  $settings = $filter->settings['htmltidy_filter_' . $format->format];

  $cleaned = htmltidy_fragment($text, TRUE, $settings, $errors, $warnings);
  $_htmltidy_filter['filtered'] = TRUE;
  $_htmltidy_filter['errors'] = $errors;
  $_htmltidy_filter['warnings'] = $warnings;
  return $cleaned;
}

/**
 * Implementation of hook_filter_FILTER_tips().
 */
function htmltidy_filter_htmltidy_tips($filter, $format, $long) {
  if ($long) {
    return t('Submitted HTML will be sanitized and cleaned for compliance automatically before output.');
  }
  else {
    return t('Submitted HTML will be sanitized and cleaned for compliance automatically before output.');
  }
}

/**
 * Implementation of hook_filter_FILTER_settings().
 */
function htmltidy_filter_htmltidy_settings($form, &$form_state, $filter, $format, $defaults, $filters) {
  $format = $format->format; // in drupal 7 $format become object

  if(!empty($filter->settings["htmltidy_filter_$format"])){
    $htmltidy_settings = array_merge(htmltidy_default_settings(), $filter->settings["htmltidy_filter_$format"]);
  }else{
    $htmltidy_settings = htmltidy_default_settings();
  }

  $settings["htmltidy_filter_$format"] = array(
    '#type'        => 'fieldset',
    '#title'       => t('HTMLTidy filter'),
    '#tree'        => TRUE,
    '#collapsible' => TRUE,
    '#description' => t("
          <p>
            Here you can set up HTML tidying options.
            This allows you to clean up the HTML that Drupal emits,
            with indenting and word-wrapping options.
          </p><p>
            The recommended way of using the HTMLTidy module is to apply it as a
            <em>validator</em> over <em>input</em>.
            This means that invalid HTML never even makes it into the system.
            To force compliance on an existing website however,
            you may need to enable the <em>output</em> filter instead.
            This approach will tidy the entire page every time.
            An alternative solution is to use HTMLTidy as an output filter.
            This means that the contents of nodes will be validated and cached
            before display, but not the entire page.
          </p>
      "),
  );

  // Paths Fieldsets
  // Determin valid setup
  if (!htmltidy_test($message, $version)) {
    drupal_set_message('HTMLTidy executable is not available. '. $message, 'error');
    $description = t('We require the HTML Tidy binary to be available on the
        server. Please <a href="http://tidy.sourceforge.net/">download and
        install it</a> wherever you can, then tell me where to find it.');
  }
  else {
    $description = t('<p>HTMLTidy is present and correct: <pre>%tidy_version</pre></p>', array('%tidy_version' => $version));
  }
  $settings["htmltidy_filter_$format"]['paths'] = array(
    '#type'          => 'fieldset',
    '#title'         => t('Paths'),
    '#description'   => $description,
  );
  $settings["htmltidy_filter_$format"]['paths']['app'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Path to htmltidy executable'),
    '#default_value' => $htmltidy_settings['paths']['app'],
    '#description'   => t('Enter the full path to htmltidy. e.g. /usr/local/bin/tidy')
  );
  $settings["htmltidy_filter_$format"]['paths']['config'] = array(
    '#type'          => 'textfield',
    '#title'   => t('Path to htmltidy.conf'),
    '#default_value' => $htmltidy_settings['paths']['config'],
    '#description'   => t("For options more advanced than those shown here, you can use an <a href='http://tidy.sourceforge.net/docs/quickref.html'>HTMLTidy configuration file</a>. Enter the full path here ( eg <code>%path</code> ), or leave it blank for none. The explicit options here usually take precedence over the conf file.", array('%path' => preg_replace('|\\\\|', '/', dirname(__FILE__)) ."/htmltidy.conf"))
  );

  //Formatting options
  $settings["htmltidy_filter_$format"]['format'] = array(
    '#type'          => 'fieldset',
    '#title'         => t('Formatting Options'),

    'process_input' => array(
      '#type'          => 'checkbox',
      '#title'         => t('Validate input text'),
      '#default_value' => $htmltidy_settings['format']['process_input'],
      '#description'   => t("More efficient than processing the output, we can instead run tidy over all text <em>entered</em> as node content. HTML will be corrected at 'Preview' time and only good HTML will ever be saved. Depending on the tidy options however, this may conflict slightly with the other output filters."),
  ),
    'indent' => array(
      '#type'          => 'checkbox',
      '#title'         => 'Indent output',
      '#default_value' => $htmltidy_settings['format']['indent'],
      '#description'   => t('When checked, htmltidy will indent HTML blocks. (&lt;div&gt;, &lt;p&gt;, etc.)'),
  ),
    'wordwrap' => array(
      '#type'          => 'textfield',
      '#title'         => 'Word wrap',
      '#default_value' => $htmltidy_settings['format']['wordwrap'],
      '#size'          => 3,
      '#description'   => t('Column width to wrap HTML code at.'),
  ),
    'wrapphp' => array(
      '#type'          => 'checkbox',
      '#title'         => t('wrap-php'),
      '#default_value' => $htmltidy_settings['format']['wrapphp'],
      '#description'   => t('When checked, htmltidy will wrap php pseudo-elements at the column entered above.  Naturally, you must set the wrap column before this will do anything.'),
  ),
    'tidymark' => array(
      '#type'          => 'checkbox',
      '#title'         => 'tidy-mark',
      '#default_value' => $htmltidy_settings['format']['tidymark'],
      '#description'   => t('When checked, htmltidy will include a &lt;meta&gt; tag specifying that htmltidy was used to generate the HTML. This has no effect if the &lt;meta&gt; tag is already specified.'),
  ),
    'clean' => array(
      '#type'          => 'checkbox',
      '#title'         => 'clean',
      '#default_value' => $htmltidy_settings['format']['clean'],
      '#description'   => t('Removes surplus tags and attributes, eliminating FONT tags and other, replacing them with style rules and structual markup. Be cautioned that turning this setting on will most likely break parts of Drupal (most notably the book module), and the automatically named style rules may simply not work.'),
  ),
    'xhtml' => array(
      '#type'          => 'checkbox',
      '#title'         => 'output-xhtml',
      '#default_value' => $htmltidy_settings['format']['xhtml'],
      '#description'   => t('Generate XHTML content.  This will set the doctype and namespace to the appropriate XHTML spec.  Note that you need to set the doctype below to actually validate against an XHTML DTD.'),
  ),
    'doctype' => array(
      '#type'          => 'textfield',
      '#title'         => 'doctype',
      '#default_value' => $htmltidy_settings['format']['doctype'],
      '#size'          => 25,
      '#maxlength'     => 25,
      '#description'   => t('Enter the doctype declaration that tidy will generate and validate against (if generating XHTML).  Valid options include: omit, auto, strict, loose, and any valid formal public identifier (don\'t try this if you are unsure what that means).'),
  ),
    'enclosetext' => array(
      '#type'          => 'checkbox',
      '#title'         => 'enclose-text',
      '#default_value' => $htmltidy_settings['format']['enclosetext'],
      '#description'   => t('Tidy will enclose any text found in the body element with &lt;p&gt; tags.  This lets you use stylesheets with greater control, fixes margins, and is required if you want valid XHTML.'),
  ),
    'encloseblocktext' => array(
      '#type'          => 'checkbox',
      '#title'         => 'enclose-block-text',
      '#default_value' => $htmltidy_settings['format']['encloseblocktext'],
      '#description'   => t('Just like the above option, but applies to any text found in an element that allows mixed content for HTML Transitional but not HTML Strict.'),
  ),
    'wordcleanup' => array(
      '#type'          => 'checkbox',
      '#title'         => 'word-2000',
      '#default_value' => $htmltidy_settings['format']['wordcleanup'],
      '#description'   => t('This option specifies if Tidy should go to great pains to strip out all the surplus stuff Microsoft Word 2000 inserts when you save Word documents as "Web pages".'),
  ),
  );

  $settings["htmltidy_filter_$format"]['debug'] = array(
    '#type'            => 'fieldset',
    '#title'           => t('Debug Options'),
    '#collapsible'     => TRUE,
    '#collapsed'       => TRUE,

    'warnings' => array(
      '#type'          => 'checkbox',
      '#title'         => t('Append errors and warnings'),
      '#default_value' => $htmltidy_settings['debug']['warnings'],
      '#description'   => t('When checked, errors, warnings and info from htmltidy will be appended to the end of pages, but only for users in <a href="%admin-user-role">roles</a> with the <strong>use htmltidy debug mode</strong> <a href="%admin-user-permission">permission flag</a> set.<br />This is useful for catching non-XHTML compliant document errors, for example.',
  array('%admin-user-role' => url('admin/user/role'), '%admin-user-permission' => url('admin/user/permission'))),
  ),
    'verbose' => array(
      '#type'          => 'checkbox',
      '#title'         => t('Verbose'),
      '#default_value' => $htmltidy_settings['debug']['verbose'],
      '#description'   => t('Be more verbose (describe what warnings/errors mean in footer).'),
  ),
    'runtwice' => array(
      '#type'          => 'checkbox',
      '#title'         => t('Run twice'),
      '#default_value' => $htmltidy_settings['debug']['runtwice'],
      '#description'   => t('This gets the line numbers on the warnings right, but is slower.  This applies only if debug mode is on.'),
  ),
  );
  return $settings;
}

