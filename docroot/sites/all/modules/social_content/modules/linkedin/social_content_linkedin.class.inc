<?php
/**
 * @file
 * Social Content Linkedin class.
 */

class SocialContentLinkedin extends SocialContent {

  /**
   * The label for this global.
   *
   * @return string
   *   The label.
   */
  public function getLabel() {
    return t('Linkedin');
  }

  /**
   * The machine name for the global.
   *
   * @return string
   *   The machine name.
   */
  public function getMachineName() {
    return 'linkedin';
  }

  /**
   * Get the source being used to get the rows i.e. account / hashtag.
   *
   * @return string
   *   The Company ID being used to fetch the rows.
   */
  public function getSource() {
    return $this->settings['instance']['company_id'];
  }

  /**
   * Fields to save from the row.
   *
   * Get fields to save.
   *
   * @return array
   *   List of fields to save.
   */
  public function fields() {
    return array(
      'id' => 'field_linkedin_external_id',
      'created' => 'created',
      'caption' => 'body',
      'account_id' => 'field_linkedin_account_id',
      'account_name' => 'field_linkedin_account_name',
      'account_link' => 'field_linkedin_account_link',
      'link' => 'field_linkedin_link',
      'picture' => 'field_linkedin_picture',
    ) + parent::fields();
  }
  /**
   * The shared global settings form for all Linkedin instances.
   *
   * @return array
   *   Global settings form.
   */
  public function globalSettingsForm() {
    $settings = $this->settings['global'];
    $token_generator_link = '';
    if (!empty($settings['client_id'])) {
      $token_generator_link = t('!token_link. You will need to add %site_url as an Authorized Redirect URL in your Linkedin app settings.', array(
        '!token_link' => l(t('Click here to generate an Access Token'), $this->getAccessTokenUrl()),
        '%site_url' => url(current_path(), array(
          'absolute' => TRUE,
        )),
      ));
    }

    // Make the token request if it comes from Linkedin.
    if (isset($_GET['code']) && empty($_POST['form_token'])) {
      $access_token = $this->getAccessToken($settings, $_GET['code']);
      if ($access_token) {
        drupal_set_message(t('Use %access_token as the Access Token.', array(
          '%access_token' => $access_token,
        )));
        drupal_goto(url(current_path(), array(
          'absolute' => TRUE,
        )));
      }
    }

    $form = parent::globalSettingsForm();
    $form['description'] = array(
      '#markup' => '<p>' . t('See !doc_url.', array(
        '!doc_url' => l('linkedin.com/secure/developer', 'https://www.linkedin.com/secure/developer'),
      )) . '</p>',
    );

    $form['access_token'] = array(
      '#type' => 'textfield',
      '#title' => t('Access Token'),
      '#default_value' => isset($settings['access_token']) ? $settings['access_token'] : NULL,
      '#description' => $token_generator_link ? $token_generator_link : t('To generate an Access Token, save this form first.'),
      '#maxlength' => 300,
    );
    $form['api_url'] = array(
      '#type' => 'textfield',
      '#title' => t('API URL'),
      '#description' => t('Do not include trailing slash. Default is !url', array(
        '!url' => 'https://api.linkedin.com',
      )),
      '#default_value' => isset($settings['api_url']) ? $settings['api_url'] : 'https://api.linkedin.com',
      '#required' => TRUE,
    );
    $form['api_version'] = array(
      '#type' => 'textfield',
      '#title' => t('API version'),
      '#description' => t("i.e. 'v1'."),
      '#default_value' => isset($settings['api_version']) ? $settings['api_version'] : 'v1',
      '#required' => TRUE,
    );
    $form['oauth_url'] = array(
      '#type' => 'textfield',
      '#title' => t('OAuth 2 URL'),
      '#description' => t('Do not include trailing slash. Default is !url', array(
        '!url' => 'https://www.linkedin.com/uas/oauth2',
      )),
      '#default_value' => isset($settings['oauth_url']) ? $settings['oauth_url'] : 'https://www.linkedin.com/uas/oauth2',
      '#required' => TRUE,
    );
    $form['client_id'] = array(
      '#type' => 'textfield',
      '#title' => t('Client ID'),
      '#default_value' => isset($settings['client_id']) ? $settings['client_id'] : NULL,
      '#required' => TRUE,
    );
    $form['client_secret'] = array(
      '#type' => 'textfield',
      '#title' => t('Client secret'),
      '#default_value' => isset($settings['client_secret']) ? $settings['client_secret'] : NULL,
      '#required' => TRUE,
    );
    return $form;
  }

  /**
   * Instance settings form.
   *
   * @return array
   *   Any instance settings that will be included on all
   *    instance forms for the current global.
   */
  public function instanceSettingsForm() {
    $settings = $this->settings['instance'];
    $form = parent::instanceSettingsForm($settings);

    $form['company_id'] = array(
      '#type' => 'textfield',
      '#title' => t('Linkedin Company ID'),
      '#description' => t("The Linkedin company id to pull the statuses from e.g. '165106'. You can use !link.", array(
        '!link' => "<a target='_blank' href='https://developer.linkedin.com/apply-getting-started#company-lookup'>Linkedin Company ID lookup</a>",
      )),
      '#default_value' => isset($settings['company_id']) ? $settings['company_id'] : NULL,
      '#required' => TRUE,
    );

    return $form;
  }

  /**
   * Get the rows to import.
   *
   * @param mixed $last_id
   *   The id of the last import.
   *
   * @return array
   *   Array with the rows.
   */
  public function getRows($last_id = NULL) {

    $settings = $this->settings['instance'];
    $global_settings = $this->settings['global'];

    if (empty($global_settings['access_token'])) {
      drupal_set_message(t('Error: Access Token is empty.'), 'error');
    }
    else if (!empty($settings['company_id'])) {

      // TODO: Job postings and products have a different format.
      $params = array(
        'format' => 'json',
        'oauth2_access_token' => $global_settings['access_token'],
        'event-type' => 'status-update',
      );

      if (!empty($settings['limit'])) {
        $params['count'] = $settings['limit'];
      }

      $url = $global_settings['api_url'] . '/' . $global_settings['api_version'] . '/companies/' . $settings['company_id'] . '/updates';
      $result = $this->httpRequest(url($url, array(
        'query' => $params,
        'external' => TRUE,
      )));
      if ($result->code == 200) {
        $posts = json_decode($result->data);
        if (isset($posts->values)) {
          return $posts->values;
        }
      }
      else {
        drupal_set_message(t('Error: @error', array('@error' => $result->data)), 'error');
      }
    }

    return array();
  }

  /**
   * Do the uploads and attach expected fields to a row about to be imported.
   */
  public function prepareRow($row) {

    $mappings = $this->getFieldMappings();

    if (isset($row->updateContent)) {

      // Status update.
      $row->id = $row->updateContent->companyStatusUpdate->share->id;
      $row->caption = $row->updateContent->companyStatusUpdate->share->comment;
      $row->created = $row->updateContent->companyStatusUpdate->share->timestamp / 1000;
      $row->account_id = $row->updateContent->company->id;
      $row->account_name = $row->updateContent->company->name;

      if (parent::prepareRow($row) === FALSE) {
        return FALSE;
      }

      // @todo: Company name.
      $row->account_link = 'https://www.linkedin.com/company/' . $row->account_id;

      $row->link = $row->updateContent->companyStatusUpdate->share->content->submittedUrl;

      $image_url = $row->updateContent->companyStatusUpdate->share->content->submittedImageUrl;
      if ($image_url) {
        $picture = $this->saveExternalFile($image_url, $mappings['picture']);
        $row->picture = $picture ? $picture : NULL;
      }

      //@todo: Link preview.

      return TRUE;
    }

    return FALSE;
  }


  /**
   * Get url to request an access token.
   *
   * @return string
   *   The url to get an access token.
   */
  public function getAccessTokenUrl() {

    $global_settings = $this->settings['global'];

    // @todo: Check state later.
    $query = array(
      'format' => 'json',
      'client_id' => $global_settings['client_id'],
      'redirect_uri' => url(current_path(), array(
        'absolute' => TRUE,
      )),
      'response_type' => 'code',
      'state' => substr(md5(microtime()), rand(0, 26), 15),
    );

    return url($global_settings['oauth_url'] . '/authorization', array('query' => $query, 'external' => TRUE));
  }

  /**
   * Get access token from Linkedin.
   *
   * @param array $settings
   *   Settings used to get access token.
   * @param string $code
   *   Code from the authorize request.
   *
   * @return string|NULL
   *   The token code, or NULL on error.
   */
  public function getAccessToken($settings, $code) {

    $fields = array(
      'format' => 'json',
      'client_id' => $settings['client_id'],
      'client_secret' => $settings['client_secret'],
      'redirect_uri' => url(current_path(), array(
        'absolute' => TRUE,
      )),
      'grant_type' => 'authorization_code',
      'code' => $code,
    );
    $options = array(
      'headers' => array(
        'Content-Type' => 'application/x-www-form-urlencoded',
      ),
      'method' => 'POST',
      'data' => drupal_http_build_query($fields),
    );
    $result = drupal_http_request($settings['oauth_url'] . '/accessToken', $options);
    $data = json_decode($result->data);
    if ($result->code != 200) {
      drupal_set_message(t('Error: @error', array('@error' => $result->data)), 'error');
      return NULL;
    }

    return $data->access_token;
  }
}
