<?php

/**
 * @file
 * Plugin for communication with Symfonie API.
 *
 * Url: http://www.moravia.com/
 */

/**
 * Translator plugin container.
 */
class TMGMTMoraviaPluginController extends TMGMTDefaultTranslatorPluginController {

  // Symfonie URL.
  protected $urlApi = 'http://symfonieapiv2.apiary-mock.com';

  /**
   * Checks whether a translator is available.
   *
   * @param TMGMTTranslator $translator
   *   The translator entity.
   *
   * @return bool
   *   TRUE if the translator plugin is available, FALSE otherwise.
   */
  public function isAvailable(TMGMTTranslator $translator) {
    if ($translator->getSetting('symfonie_token')) {
      return TRUE;
    }
    return FALSE;
  }

  /**
   * Return a reason why the translator is not available.
   *
   * @param TMGMTTranslator $translator
   *   The translator entity.
   *
   *   Might be called when isAvailable() returns FALSE to get a reason that
   *   can be displayed to the user.
   *
   * @todo Remove this once http://drupal.org/node/1420364 is done.
   */
  public function getNotAvailableReason(TMGMTTranslator $translator) {
    return "";
  }

  /**
   * Settings checkout.
   *
   * @param TMGMTJob $job
   *   TMGMTJob instance.
   *
   * @return bool
   *   Result of check.
   */
  public function hasCheckoutSettings(TMGMTJob $job) {
    return TRUE;
  }

  /**
   * Returns project list.
   *
   * @param TMGMTTranslator $translator
   *   TMGMTTranslator instance.
   *
   * @return array
   *   Project list.
   */
  public function getProjects(TMGMTTranslator $translator) {
    $result = tmgmt_moravia_symfonie_api_call($translator, "/api/projects", "GET");
    $projects_result = json_decode($result->data);
    $projects = array();
    foreach ($projects_result->projects as $project) {
      $projects[$project->id] = $project->name;
    }
    return $projects;
  }

  /**
   * Send translate task to Symfonie.
   *
   * @param TMGMTJob $job
   *   TMGMTJob instance.
   *
   * @return TmgmtMoraviaSymfonieTask
   *   TmgmtMoraviaSymfonieTask instance.
   */
  public function jobToSymfonieTask(TMGMTJob $job) {

    $task = TmgmtMoraviaSymfonieTask::load($job->getTranslator(), $job->reference);
    $task->job = $job;
    return $task;
  }

  /**
   * Submits the translation request and sends it to the translation provider.
   *
   * @param TMGMTJob $job
   *   The job that should be submitted.
   *
   * @ingroup tmgmt_remote_languages_mapping
   */
  public function requestTranslation(TMGMTJob $job) {
    $task = TmgmtMoraviaSymfonieTask::createFromJob($job);
    if ($task) {
      $task->save($job->getTranslator());
      $job->reference = $task->getId();
      $job->submitted();
      $job->save();
    }
  }

  /**
   * Import xliff document.
   *
   * @param string $xliff
   *   Document.
   *
   * @return bool
   *   Result of import.
   */
  public function importXliff($xliff) {
    $xliff_processor = new TMGMTFileformatXLIFF();
    $filename = sys_get_temp_dir() . '/' . uniqid() . ".xliff";
    file_put_contents($filename, $xliff);
    $result = $xliff_processor->validateImport($filename);

    if (!$result) {
      throw new Exception("XLIFF Validation problem", 1);
    }

    $result = $xliff_processor->import($filename);
    unlink($filename);
    return $result;

  }

  /**
   * Cancel translation task.
   *
   * @param TMGMTJob $job
   *   TMGMTJob instance.
   *
   * @return bool
   *   Result of cancel request.
   */
  public function abortTranslation(TMGMTJob $job) {
    $translator = $job->getTranslator();
    $task_id = $job->reference;

    $job->aborted();

    tmgmt_moravia_symfonie_api_call($translator, "/api/task/actions?id={$task_id}&command=Cancel&description=", "POST", array());
    return TRUE;
  }

}

/**
 * Function for communicating with Symfonie api.
 *
 * @param TMGMTTranslator $translator
 *   TMGMTTranslator instance.
 * @param string $url
 *   Method url.
 * @param string $method
 *   Request method.
 * @param mixed $data
 *   Request data.
 * @param array $user_options
 *   User options.
 *
 * @return array
 *   Request result.
 */
function tmgmt_moravia_symfonie_api_call(TMGMTTranslator $translator, $url, $method, $data = NULL, $user_options = array()) {
  $default_options = array(
    "method" => $method,
    "headers" => array(
      "Authorization" => "authToken " . $translator->getSetting('symfonie_token'),
      "Content-Type" => "application/json; charset=UTF-8",
    ),
  );

  $options = array_merge_recursive($default_options, $user_options);
  if ($data) {
    $options['data'] = $data;
  }
  $result = drupal_http_request("https://symfonie.moravia.com$url", $options);
  return $result;
}

/**
 * Symfonie task abstraction.
 */
class TmgmtMoraviaSymfonieTask {

  protected $title;
  protected $project;
  protected $sourceLanguageId;
  protected $targetLanguageId;
  protected $dueDate;
  protected $stateId;
  protected $id = NULL;
  public $attachments = array();
  public $job = NULL;

  /**
   * Set title.
   *
   * @param string $title
   *   Task title.
   */
  public function setTitle($title) {
    $this->title = $title;
  }

  /**
   * Get state ID.
   *
   * @return int
   *   Id.
   */
  public function getStateId() {
    return $this->stateId;
  }

  /**
   * Load Symfonie api task response.
   *
   * @param TMGMTTranslator $translator
   *   TMGMTTranslator translator.
   * @param int $id
   *   Id.
   *
   * @return TmgmtMoraviaSymfonieTask
   *   TmgmtMoraviaSymfonieTask.
   */
  public static function load(TMGMTTranslator $translator, $id) {
    $result = tmgmt_moravia_symfonie_api_call($translator, "/api/tasks/$id", "GET");
    $symfonie_data = json_decode($result->data);

    $task = new self();
    $task->title = $symfonie_data->task->name;
    $task->project = $symfonie_data->task->project_id;
    $task->sourceLanguageId = $symfonie_data->task->source_language_id;
    $task->targetLanguageId = $symfonie_data->task->language_ids['0'];
    $task->id = $symfonie_data->task->id;
    $task->stateId = $symfonie_data->task->state_id;
    foreach ($symfonie_data->task->attachments as $a_id) {
      $task->attachments[] = TmgmtMoraviaSymfonieTaskAttachment::load($translator, $a_id);
    }

    return $task;
  }

  /**
   * Create xliff document.
   *
   * @param TMGMTJob $job
   *   TMGMTJob job.
   */
  public static function createXliff(TMGMTJob $job) {
    $xliff_processor = new TMGMTFileformatXLIFF();
    return $xliff_processor->export($job);
  }

  /**
   * Create task from job.
   *
   * @param TMGMTJob $job
   *   TMGMTJob job.
   *
   * @return TmgmtMoraviaSymfonieTask
   *   Symfonie task.
   */
  public static function createFromJob(TMGMTJob $job) {
    $translator = $job->getTranslator();
    $task = new TmgmtMoraviaSymfonieTask();

    // Job reference is equal to ID of remote symfonie task.
    if ($job->reference) {
      $task->id = $job->reference;
    }

    $task->title = $job->settings['moravia_task_name'];
    $task->project = $job->settings['moravia_project'];
    $due_date = $job->settings['moravia_task_due_date']['date']['year'] . "-" .
    $job->settings['moravia_task_due_date']['date']['month'] . "-" .
    $job->settings['moravia_task_due_date']['date']['day'] . " " .
    $job->settings['moravia_task_due_date']['time'] . ":00:00";
    $due_date = gmdate("Y-m-d\TH:i:s\Z", strtotime($due_date));
    $task->dueDate = $due_date;

    self::createXliff($job);

    $task->sourceLanguageId = TmgmtMoraviaLanguageMapper::ietfToId($translator, $job->source_language);
    $task->targetLanguageId = TmgmtMoraviaLanguageMapper::ietfToId($translator, $job->target_language);

    if (!$task->sourceLanguageId) {
      $job->rejected(t('Source language mapping problem.'), array(), 'error');
      return FALSE;
    }
    if (!$task->targetLanguageId) {
      $job->rejected(t('Target language mapping problem.'), array(), 'error');
      return FALSE;
    }

    // TODO? Drupal tag = id 810, may be better than ask for the same thing
    // over and over again with the same result.
    $task->tag_ids = array(810);
    $task->xliff = self::createXliff($job);
    $task->job = $job;
    return $task;
  }

  /**
   * Get ID.
   *
   * @return int
   *   Id.
   */
  public function getId() {
    return $this->id;
  }

  /**
   * Save task.
   *
   * @param TMGMTTranslator $translator
   *   Translator instance.
   */
  public function save(TMGMTTranslator $translator) {

    if (!$this->id) {
      // Set project manager to task automatically.
      $project_manager = tmgmt_moravia_symfonie_api_call($translator, "/api/projects/{$this->project}/projectmanager", "GET");
      $backup_managers = tmgmt_moravia_symfonie_api_call($translator, "/api/projects/{$this->project}/backupmanagers", "GET");

      $project_manager = json_decode($project_manager->data);
      $backup_managers = json_decode($backup_managers->data);

      $asignees = array();

      if (isset($project_manager->users[0]) && $project_manager->users[0]->id) {
        $asignees[] = $project_manager->users[0]->id;
      }
      if (isset($backup_managers->users) && $backup_managers->users) {
        foreach ($backup_managers->users as $user) {
          $asignees[] = $user->id;
        }
      }
    }

    $request_array = array(
      "task" => array(
        "name" => $this->title,
        "source_language_id" => $this->sourceLanguageId,
        "language_ids" => array($this->targetLanguageId),
        "due_date" => $this->dueDate,
        "project_id" => $this->project,
        "tag_ids" => $this->tag_ids,
        "user_ids" => $asignees,
      ),
    );

    $json_request = json_encode($request_array);

    if ($this->id) {
      $result = tmgmt_moravia_symfonie_api_call($translator, "/api/tasks/{$this->id}", "PUT", $json_request);
      $edit = TRUE;
    }
    else {
      $result = tmgmt_moravia_symfonie_api_call($translator, "/api/tasks", "POST", $json_request);
      $edit = FALSE;
    }

    $data = json_decode($result->data);
    $this->id = $data->task->id;

    $result = tmgmt_moravia_symfonie_api_call($translator,
      "/api/taskattachment/Upload?taskId={$this->id}&type=Source&qqfile=1.xliff", "POST",
      $this->xliff,
      array(
        "headers" => array(
          "X-Mime-Type" => "text/plain",
          "X-File-Name" => "1.xliff",
          "X-Requested-With" => "XMLHttpRequest",
        ),
      )
    );

    $result = tmgmt_moravia_symfonie_api_call($translator, "/api/tasks/{$this->id}", "PUT", $json_request);

    if (!$edit) {
      // Set ordered state.
      tmgmt_moravia_symfonie_api_call($translator, "/api/task/actions?id={$this->id}&command=Order&description=", "POST", array());
    }
  }
}

/**
 * Class for working with Symfonie attachments.
 */
class TmgmtMoraviaSymfonieTaskAttachment {

  public $id;
  public $fileType;

  /**
   * Init class.
   *
   * @param TMGMTTranslator $translator
   *   Translator instance.
   */
  public function __construct(TMGMTTranslator $translator) {
    $this->translator = $translator;
  }

  /**
   * Load attachment.
   *
   * @param TMGMTTranslator $translator
   *   Translator instance.
   * @param int $id
   *   Id.
   *
   * @return TmgmtMoraviaSymfonieTaskAttachment
   *   Attachment.
   */
  public static function load(TMGMTTranslator $translator, $id) {
    $data = self::loadDataFromAPI($translator, $id);
    $attachment = new self($translator);
    $attachment->id = $data->id;
    $attachment->fileType = $data->file_type;
    return $attachment;
  }

  /**
   * Load data from Symfonie api.
   *
   * @param TMGMTTranslator $translator
   *   Translator instance.
   * @param int $id
   *   Id.
   *
   * @return XLIFF
   *   Xliff document.
   */
  protected static function loadDataFromAPI(TMGMTTranslator $translator, $id) {
    $result = tmgmt_moravia_symfonie_api_call($translator, "/api/taskattachment/$id", "GET");

    $attachment = json_decode($result->data);
    return $attachment->taskattachment;
  }

  /**
   * Download file from Symfonie api.
   *
   * @return array
   *   File data.
   */
  public function downloadFile() {
    $result = tmgmt_moravia_symfonie_api_call($this->translator,
      "/api/taskattachment/Download?attachmentId={$this->id}", "GET");
    return $result->data;
  }

}

/**
 * Class for mapping languages.
 */
class TmgmtMoraviaLanguageMapper {

  public static $map = NULL;

  /**
   * Get map.
   *
   * @param TMGMTTranslator $translator
   *   Translator instance.
   */
  private static function getMap(TMGMTTranslator $translator) {
    if (self::$map == NULL) {
      $result = tmgmt_moravia_symfonie_api_call($translator, "/api/languages", "GET");

      $languages  = json_decode($result->data)->languages;

      self::$map = array();
      foreach ($languages as $language) {
        self::$map[$language->ietf] = $language->id;
      }
    }
    return self::$map;
  }

  /**
   * Transform ietf to ID.
   *
   * @param TMGMTTranslator $translator
   *   Translator instance.
   * @param string $code
   *   Code.
   *
   * @return int
   *   Id.
   */
  public static function ietfToId(TMGMTTranslator $translator, $code) {
    $map = self::getMap($translator);
    $translated_code = $translator->settings['remote_languages_mappings'][$code];
    return $map[$translated_code];
  }

}
