<?php
/**
 * @file
 * Context reaction for Content Experiments.
 */

class content_experiments_context_reaction extends context_reaction {
   /**
   * Editor form.
   */
  function editor_form($context) {
    $form = $this->options_form($context);
    return $form;
  }

  /**
   * Submit handler for editor form.
   */
  function editor_form_submit($context, $values) {
    return $values;
  }

  /**
   * Allow admins to provide a section title, section subtitle and section class.
   */
  function options_form($context) {
    $values = $this->fetch_from_context($context);
    $form = array(
      '#tree' => TRUE,
      '#title' => t('Content Experiments'),
      'content_experiments_key' => array(
        '#title' => t('Experiment key'),
        '#description' => t('The experiment key comes from Google Analytics. It should appear under the code that Google Analytics instructs you to paste immediately after the opening of the &lt;head&gt; tag.'),
        '#type' => 'textfield',
        '#maxlength' => 255,
        '#default_value' => isset($values['content_experiments_key']) ? $values['content_experiments_key'] : '',
      ),
    );
    return $form;
  }

  /**
   * Output a list of active contexts.
   */
  function execute(&$vars) {
    foreach ($this->get_contexts() as $k => $v) {
      if (!empty($v->reactions[$this->plugin]['content_experiments_key'])) {
        $code = $v->reactions[$this->plugin]['content_experiments_key'];
        // Add Google Analytics Experiments javascript
        $inline_script = <<<EOL
          <!-- Google Analytics Content Experiment code -->
<script>function utmx_section(){}function utmx(){}(function(){var
k='$code',d=document,l=d.location,c=d.cookie;
if(l.search.indexOf('utm_expid='+k)>0)return;
function f(n){if(c){var i=c.indexOf(n+'=');if(i>-1){var j=c.
indexOf(';',i);return escape(c.substring(i+n.length+1,j<0?c.
length:j))}}}var x=f('__utmx'),xx=f('__utmxx'),h=l.hash;d.write(
'<sc'+'ript src="'+'http'+(l.protocol=='https:'?'s://ssl':
'://www')+'.google-analytics.com/ga_exp.js?'+'utmxkey='+k+
'&utmx='+(x?x:'')+'&utmxx='+(xx?xx:'')+'&utmxtime='+new Date().
valueOf()+(h?'&utmxhash='+escape(h.substr(1)):'')+
'" type="text/javascript" charset="utf-8"><\/sc'+'ript>')})();
</script><script>utmx('url','A/B');</script>
<!-- End of Google Analytics Content Experiment code -->
EOL;
        $element = array(
          '#type' => 'markup',
          '#markup' => $inline_script,
        );
        drupal_add_html_head($element, 'content_experiments_context');
      }
    }
  }
}
