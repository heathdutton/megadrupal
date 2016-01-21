<?php

namespace Drupal\campaignion_wizard;

/**
 * @TODO: This class shouldn't be part of the wizard!
 */
class Email {
  protected $form_id;
  protected $eid;
  protected $node;
  protected $is_new;

  public function __construct($node, $form_id, $eid) {
    $this->node    = $node;
    $this->form_id = $form_id;
    $this->eid     = $eid;
    $this->is_new  = !isset($node->webform['emails'][$eid]);
  }

  protected function &getEmailForm(&$form_state) {
    if ( !$this->is_new ) {
      $email = webform_email_load($this->eid, $this->node->nid);
    } else {
      // if this is a new petition we set email text to be HTML by default
      $email = webform_email_load('new', $this->node->nid);
      $email['html'] = TRUE;
    }

    $formats = array(
      'plain' => variable_get('campaignion_wizard_text_format_plain', 'plain_text'),
      'html'  => variable_get('campaignion_wizard_text_format_html', 'full_html_with_editor'),
    );

    $email_form = webform_email_edit_form(array(), $form_state, $this->node, $email);
    $email_form['#theme'] = 'campaignion_wizard_email_form';
    unset($email_form['actions']);

    // find email component and force sending email to the form submitter if it exists
    foreach ($this->node->webform['components'] as $key => $component) {
      if ($component['form_key'] == 'email') {
        $email_form['email_option']   ['#default_value'] = 'component';
        $email_form['email_component']['#default_value'] = $key;
        $email_form['email_option']   ['#access'] = FALSE;
        $email_form['email_custom']   ['#access'] = FALSE;
        $email_form['email_component']['#access'] = FALSE;
        break;
      }
    }

    $email_form['subject_option']['#default_value'] = 'custom';
    $email_form['subject_option']['#access'] = FALSE;
    $email_form['subject_component']['#access'] = FALSE;
    $email_form['subject_custom']['#title'] = t('Subject');
    $email_form['subject_custom']['#access'] = TRUE;

    $email_form['from_name_option']['component']['#access'] = FALSE;
    unset($email_form['from_name_option']['#options']['component']);
    unset($email_form['from_name_component']);

    unset($email_form['from_address_option']['#options']['component']);
    unset($email_form['from_address_component']);

    $email_form['template']['#collapsible'] = FALSE;
    $email_form['template']['#title'] = t('Your email');
    $email_form['template']['#description']  = "";
    $email_form['template']['components']['#access'] = FALSE;
    $email_form['template']['template_option']['#default_value'] = 'custom';
    $email_form['template']['template_option']['#access'] = FALSE;
    $email_form['template']['html']['#access'] = TRUE;
    $email_form['template']['attachments']['#access'] = FALSE;
    $email_form['template']['#tree'] = TRUE;
    $email_form['template']['template']['#base_type']    = 'textarea';
    $email_form['template']['template']['#type']         = 'text_format';
    $email_form['template']['template']['#format']       = $email['html'] ? $formats['html'] : $formats['plain'];
    $email_form['template']['template']['#wysiwyg']      = TRUE;
    $email_form['template']['template']['#pre_render'][] = 'wysiwyg_pre_render_text_format';
    // needed for ['template']['tokens'] which does not load js via #collapsible
    // tokens it is only html markup
    drupal_add_library("system", "drupal.collapse");
    
    $settings['webform']['textFormat'] = $formats;
    $email_form['#attached']['js'][] = array(
      'type' => 'setting',
      'data' => $settings,
    );
    $email_form['#attached']['js'][] = drupal_get_path('module', 'campaignion_wizard') . '/js/email-text-format.js';

    return $email_form;
  }

  public function form($messages, &$form_state) {
    $form[$this->form_id . '_toggle'] = array(
      '#type'        => 'fieldset',
    );

    $checkbox_id = drupal_html_id($this->form_id . '_checkbox');
    $form[$this->form_id . '_toggle'][$this->form_id . '_check'] = array(
      '#type'          => 'checkbox',
      '#title'         => $messages['toggle_title'],
      '#id'            => $checkbox_id,
      '#return_value'  => 1,
      '#default_value' => !$this->is_new ? 1 : 0,
    );

    $form[$this->form_id . '_email'] = array(
      '#type'        => 'fieldset',
      '#title'       => $messages['email_title'],
      '#attributes'  => array('class' => array('email-wrapper')),
      '#states' => array(
        'visible' => array(   // action to take.
          "#$checkbox_id" => array('checked' => TRUE),
        ),
      ),
    ) + $this->getEmailForm($form_state);

    return $form;
  }

  public function validate($form, &$form_state) {
    $values = &$form_state['values'];

    /* validate thank you */
    $form_state['values'] = &$values[$this->form_id . '_email'];
    foreach ($form_state['values']['template'] as $key => &$val) {
      $form_state['values'][$key] = &$val;
    }
    webform_email_edit_form_validate($form, $form_state);

    $form_state['values'] = &$values;
  }

  public function submit($form, &$form_state, $email_type) {
    $values = &$form_state['values'];

    if ($values[$this->form_id . '_toggle'][$this->form_id . '_check'] == 1) {
      $values[$this->form_id . '_email']['template'] = &$values[$this->form_id . '_email']['template']['value'];
      $form_state['values'] =& $values[$this->form_id . '_email'];

     $this->saveEmail($email_type, $form, $form_state);

      $form_state['values'] = &$values;
    } else {
      if (!$this->is_new) {
        $this->deleteEmail();
      }
    }
  }

  /*
   * save or update an email from raw data instead of a filled out form
   * array
   */
  public function submitData($email_data, $email_type) {

    if (isset($email_data['template']) == TRUE) {
      $email_data['template'] = str_replace(array("\r", "\n"), array('', "\n"), $email_data['template']);
    }

    foreach (array('email', 'from_name', 'from_address', 'subject', 'template') as $email_field) {
      if (isset($email_data[$email_field]) == FALSE) {
        $email_data[$email_field] = 'default';
      }
    }
    $email_data['html']        = empty($email_data['html'])        == TRUE ? 0 : 1;
    $email_data['attachments'] = empty($email_data['attachments']) == TRUE ? 0 : 1;

    if (isset($email_data['components']) == FALSE) {
      $email_data['components'] = $this->node->webform['components'];
    }
    $included = array_keys(array_filter((array) $email_data['components']));
    $excluded = array_diff(array_keys($this->node->webform['components']), $included);
    $email_data['excluded_components'] = $excluded;

    if ($this->is_new == TRUE) {
      webform_email_insert($email_data);
    }
    else {
      webform_email_update($email_data);
    }

    db_merge('webform_confirm_email')
      ->key(array(
        'nid' => $email_data['nid'],
        'eid' => $email_data['eid'],
      ))
      ->fields(array('email_type' => $email_type))
      ->execute();
  }

  /*
   * save or update an email from form data
   */
  protected function saveEmail($email_type, &$form, &$form_state) {
    if ($this->is_new) {
      $email = array(
        'eid' => (int) $this->eid,
        'nid' => (int) $this->node->nid,
        'excluded_components' => array(),
      );
      // if eid is set webform_email_edit_form_submit tries to do a db_update
      // so we insert an empty email first.
      $form_state['values']['eid'] = webform_email_insert($email);
    }

    webform_email_edit_form_submit($form, $form_state);

    db_merge('webform_confirm_email')
      ->key(array(
        'nid' => $this->node->nid,
        'eid' => $this->eid,
      ))
      ->fields(array('email_type' => $email_type))
      ->execute();
  }

  protected function deleteEmail() {
    $nid = $this->node->nid;
    $eid = $this->eid;

    // MySQL only but simple and fast
    db_query(
      'DELETE wceconf, wcecode ' .
      '  FROM {webform_confirm_email_confirmation_emails} wceconf ' .
      '  INNER JOIN {webform_confirm_email_code} wcecode ' .
      '    ON wcecode.nid = wceconf.nid ' .
      '    AND wcecode.sid = wceconf.sid ' .
      '  WHERE wcecode.nid = :nid ' .
      '  AND   wcecode.eid = :eid' ,
      array(
        ':nid' => (int) ($nid),
        ':eid' => (int) ($eid),
      )
    );

    db_delete('webform_confirm_email')
      ->condition('nid', $nid)
      ->condition('eid', $eid)
      ->execute();

    webform_email_delete($this->node, array('eid' => $eid));
  }
}
