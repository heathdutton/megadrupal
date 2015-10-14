<?php
/**
 * @file
 * Seetings page for the Share This Thing module.
 */

/**
 * Module settings form.
 */
function share_this_thing_admin_settings($form, &$form_state) {
  $form['example'] = array(
    '#type' => 'item',
    '#title' => t('Example'),
    '#markup' => '<pre>
    &lt;!-- AddThis Button BEGIN --&gt;
    &lt;div class=&quot;addthis_toolbox addthis_default_style &quot;&gt;
    &lt;a class=&quot;addthis_button_facebook_like&quot; fb:like:layout=&quot;button_count&quot;&gt;&lt;/a&gt;
    &lt;a class=&quot;addthis_button_tweet&quot;&gt;&lt;/a&gt;
    &lt;a class=&quot;addthis_button_google_plusone&quot; g:plusone:size=&quot;medium&quot;&gt;&lt;/a&gt;
    &lt;a class=&quot;addthis_counter addthis_pill_style&quot;&gt;&lt;/a&gt;
    &lt;/div&gt;
    &lt;script type=&quot;text/javascript&quot;&gt;
    /**
     * AddThis Configuration options
     * @link http://support.addthis.com/customer/portal/articles/381263-addthis-client-api
     */
    var addthis_config = {
      &quot;data_track_addressbar&quot;: true
      &quot;username&quot;: abc123 // Your AddThis username (*)
    };
    &lt;/script&gt;
    &lt;script type=&quot;text/javascript&quot; src=&quot;//s7.addthis.com/js/300/addthis_widget.js#async=1&quot;&gt;&lt;/script&gt;
    &lt;script type=&quot;text/javascript&quot;&gt;
    /**
     * Let each AddThis button inherit the shared content\'s title and URL.
     */
    (function ($) {
      $("#share-this-thing-form").each(function() {
        $(".addthis_toolbox", this).attr({
          "addthis:url": $(".stt-share-link", this).val(),
          "addthis:title": $(".stt-share-link", this).attr("title")
        });
      })
    })(jQuery);

    /**
     * Re-initialize AddThis toolbox and/or counter.
     */
    if (window.addthis) {
      addthis.toolbox(".addthis_toolbox");
      addthis.counter(".addthis_counter");
    }
    &lt;/script&gt;
    &lt;!-- AddThis Button END --&gt;
    </pre>',
    '#weight' => 0,
    '#description' => t('(*) Sign up with <a href="!url">AddThis</a> to get a username.', array('!url' => 'https://www.addthis.com')),
  );

  $form['share_this_thing_markup'] = array(
    '#type' => 'textarea',
    '#rows' => 20,
    '#resizable' => TRUE,
    '#title' => t('Additional HTML'),
    '#default_value' => variable_get('share_this_thing_markup', ''),
    '#weight' => 1,
  );

  return system_settings_form($form);
}
