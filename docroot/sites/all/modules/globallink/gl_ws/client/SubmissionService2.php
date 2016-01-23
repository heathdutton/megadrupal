<?php

if (!class_exists('anonymous0')) {
  class anonymous0 {
  }
}

if (!class_exists('base64Binary')) {
  class base64Binary {
    public $_;
    public $contentType;
  }
}

if (!class_exists('hexBinary')) {
  class hexBinary {
    public $_;
    public $contentType;
  }
}

if (!class_exists('Notification')) {
  class Notification {
    public $errorMessage;
    public $notificationDate;
    public $notificationPriority;
    public $notificationText;
  }
}

if (!class_exists('NotificationPriority')) {
  class NotificationPriority {
    public $notificationPriorityName;
  }
}

if (!class_exists('Announcement')) {
  class Announcement {
    public $announcementText;
    public $date;
  }
}

if (!class_exists('ContentMonitorPluginInfo')) {
  class ContentMonitorPluginInfo {
    public $pluginId;
    public $pluginName;
  }
}

if (!class_exists('Date')) {
  class Date {
    public $critical;
    public $date;
  }
}

if (!class_exists('Document')) {
  class Document {
    public $documentGroup;
    public $documentInfo;
    public $id;
    public $sourceLanguage;
    public $sourceWordCount;
    public $ticket;
  }
}

if (!class_exists('DocumentGroup')) {
  class DocumentGroup {
    public $classifier;
    public $documents;
    public $mimeType;
    public $submission;
  }
}

if (!class_exists('DocumentInfo')) {
  class DocumentInfo {
    public $clientIdentifier;
    public $dateRequested;
    public $instructions;
    public $metadata;
    public $name;
    public $projectTicket;
    public $sourceLocale;
    public $submissionTicket;
    public $targetInfos;
    public $wordCount;
  }
}

if (!class_exists('DocumentPagedList')) {
  class DocumentPagedList {
    public $elements;
    public $pagedListInfo;
    public $tasks;
    public $totalCount;
  }
}

if (!class_exists('DocumentSearchRequest')) {
  class DocumentSearchRequest {
    public $projectTickets;
    public $sourceLocaleId;
    public $submissionTicket;
  }
}

if (!class_exists('DocumentTicket')) {
  class DocumentTicket {
    public $submissionTicket;
    public $ticketId;
  }
}

if (!class_exists('EntityTypeEnum')) {
  class EntityTypeEnum {
    public $name;
    public $value;
  }
}

if (!class_exists('FileFormatProfile')) {
  class FileFormatProfile {
    public $configurable;
    public $isDefault;
    public $mimeType;
    public $pluginId;
    public $pluginName;
    public $profileName;
    public $targetWorkflowDefinition;
    public $ticket;
  }
}

if (!class_exists('FileFormatProgressData')) {
  class FileFormatProgressData {
    public $dateCompleted;
    public $fileCount;
    public $fileFormatName;
    public $fileProgressData;
    public $jobTicket;
    public $workflowDueDate;
    public $workflowStatus;
  }
}

if (!class_exists('FileProgressData')) {
  class FileProgressData {
    public $numberOfAvailableFiles;
    public $numberOfCanceledFiles;
    public $numberOfCompletedFiles;
    public $numberOfDeliveredFiles;
    public $numberOfFailedFiles;
    public $numberOfInProcessFiles;
    public $overallProgressPercent;
  }
}

if (!class_exists('FuzzyTmStatistics')) {
  class FuzzyTmStatistics {
    public $fuzzyName;
    public $wordCount;
  }
}

if (!class_exists('ItemFolderEnum')) {
  class ItemFolderEnum {
    public $value;
  }
}

if (!class_exists('ItemStatusEnum')) {
  class ItemStatusEnum {
    public $name;
    public $value;
  }
}

if (!class_exists('Metadata')) {
  class Metadata {
    public $key;
    public $value;
  }
}

if (!class_exists('Language')) {
  class Language {
    public $locale;
    public $value;
  }
}

if (!class_exists('LanguageDirection')) {
  class LanguageDirection {
    public $sourceLanguage;
    public $targetLanguage;
  }
}

if (!class_exists('LanguageDirectionModel')) {
  class LanguageDirectionModel {
    public $dateCompleted;
    public $fileCount;
    public $fileFormatProgressData;
    public $fileProgress;
    public $sourceLanguage;
    public $targetLanguage;
    public $workflowDueDate;
    public $workflowStatus;
  }
}

if (!class_exists('PagedListInfo')) {
  class PagedListInfo {
    public $index;
    public $indexesSize;
    public $size;
    public $sortDirection;
    public $sortProperty;
  }
}

if (!class_exists('Phase')) {
  class Phase {
    public $dateEnded;
    public $dueDate;
    public $name;
    public $status;
  }
}

if (!class_exists('Priority')) {
  class Priority {
    public $name;
    public $value;
  }
}

if (!class_exists('Project')) {
  class Project {
    public $announcements;
    public $contentMonitorPluginInfo;
    public $defaultLanguageDirections;
    public $defaultTargetWorkflowDefinition;
    public $defaultTargetWorkflowDefinitionTicket;
    public $fileFormatProfiles;
    public $frequentLanguageDirections;
    public $metadata;
    public $organizationName;
    public $projectInfo;
    public $projectLanguageDirections;
    public $ticket;
    public $workflowDefinitions;
  }
}

if (!class_exists('ProjectInfo')) {
  class ProjectInfo {
    public $clientIdentifier;
    public $defaultJobWorkflowDefinitionTicket;
    public $defaultSubmissionWorkflowDefinitionTicket;
    public $defaultTargetWorkflowDefinitionTicket;
    public $enabled;
    public $name;
    public $shortCode;
  }
}

if (!class_exists('ProjectLanguage')) {
  class ProjectLanguage {
    public $customLocaleCode;
    public $localeCode;
  }
}

if (!class_exists('RepositoryItem')) {
  class RepositoryItem {
    public $data;
    public $resourceInfo;
  }
}

if (!class_exists('ResourceInfo')) {
  class ResourceInfo {
    public $classifier;
    public $clientIdentifier;
    public $description;
    public $encoding;
    public $md5Checksum;
    public $mimeType;
    public $name;
    public $path;
    public $resourceInfoId;
    public $size;
    public $type;
  }
}

if (!class_exists('ResourceType')) {
  class ResourceType {
    public $value;
  }
}

if (!class_exists('Submission')) {
  class Submission {
    public $alerts;
    public $availableTasks;
    public $dateCompleted;
    public $dateCreated;
    public $dateEstimated;
    public $documents;
    public $dueDate;
    public $id;
    public $owner;
    public $project;
    public $status;
    public $submissionInfo;
    public $submitter;
    public $submitterFullName;
    public $ticket;
    public $workflowDefinition;
  }
}

if (!class_exists('SubmissionInfo')) {
  class SubmissionInfo {
    public $clientIdentifier;
    public $dateRequested;
    public $metadata;
    public $name;
    public $projectTicket;
    public $submitter;
    public $workflowDefinitionTicket;
  }
}

if (!class_exists('SubmissionPagedList')) {
  class SubmissionPagedList {
    public $elements;
    public $pagedListInfo;
    public $tasks;
    public $totalCount;
  }
}

if (!class_exists('SimpleSubmissionSearchModel')) {
  class SimpleSubmissionSearchModel {
    public $alerts;
    public $availableTasks;
    public $date;
    public $dateCompleted;
    public $dueDate;
    public $fileCount;
    public $fileProgress;
    public $id;
    public $instructions;
    public $officeName;
    public $owner;
    public $priority;
    public $projectName;
    public $projectTicket;
    public $sourceLanguage;
    public $status;
    public $submissionName;
    public $submitterFullName;
    public $ticket;
    public $wordCount;
    public $workflowDueDate;
    public $workflowStatus;
  }
}

if (!class_exists('SubmissionSearchModelPagedList')) {
  class SubmissionSearchModelPagedList {
    public $elements;
    public $pagedListInfo;
    public $tasks;
    public $totalCount;
  }
}

if (!class_exists('SubmissionSearchRequest')) {
  class SubmissionSearchRequest {
    public $folder;
    public $projectTickets;
    public $submissionDate;
    public $submissionDueDate;
    public $submissionName;
  }
}

if (!class_exists('Target')) {
  class Target {
    public $availableTasks;
    public $dateCompleted;
    public $dateCreated;
    public $dateEstimated;
    public $document;
    public $downloadThresholdTimeStamp;
    public $dueDate;
    public $fileName;
    public $id;
    public $phases;
    public $refPhase;
    public $sourceLanguage;
    public $sourceWordCount;
    public $status;
    public $targetInfo;
    public $targetLanguage;
    public $targetWordCount;
    public $ticket;
    public $tmStatistics;
    public $workflowDefinition;
  }
}

if (!class_exists('TargetInfo')) {
  class TargetInfo {
    public $dateRequested;
    public $encoding;
    public $instructions;
    public $metadata;
    public $priority;
    public $requestedDueDate;
    public $targetLocale;
    public $workflowDefinitionTicket;
  }
}

if (!class_exists('TargetPagedList')) {
  class TargetPagedList {
    public $elements;
    public $pagedListInfo;
    public $tasks;
    public $totalCount;
  }
}

if (!class_exists('TargetSearchRequest')) {
  class TargetSearchRequest {
    public $dateCreated;
    public $folder;
    public $projectTickets;
    public $sourceLocaleId;
    public $submissionTicket;
    public $targetLocaleId;
  }
}

if (!class_exists('Task')) {
  class Task {
    public $groupName;
    public $selectStyle;
    public $taskId;
    public $taskName;
    public $weight;
  }
}

if (!class_exists('TmStatistics')) {
  class TmStatistics {
    public $fuzzyWordCount1;
    public $fuzzyWordCount10;
    public $fuzzyWordCount2;
    public $fuzzyWordCount3;
    public $fuzzyWordCount4;
    public $fuzzyWordCount5;
    public $fuzzyWordCount6;
    public $fuzzyWordCount7;
    public $fuzzyWordCount8;
    public $fuzzyWordCount9;
    public $goldWordCount;
    public $noMatchWordCount;
    public $oneHundredMatchWordCount;
    public $repetitionWordCount;
    public $totalWordCount;
  }
}

if (!class_exists('WorkflowDefinition')) {
  class WorkflowDefinition {
    public $description;
    public $name;
    public $ticket;
    public $type;
  }
}

if (!class_exists('UserInfo')) {
  class UserInfo {
    public $accountNonExpired;
    public $accountNonLocked;
    public $autoClaimMultipleTasks;
    public $claimMultipleJobTasks;
    public $credentialsNonExpired;
    public $dateLastLogin;
    public $emailAddress;
    public $emailNotification;
    public $enabled;
    public $firstName;
    public $lastName;
    public $password;
    public $timeZone;
    public $userName;
    public $userType;
  }
}

if (!class_exists('TiUserInfo')) {
  class TiUserInfo {
    public $languageDirections;
    public $organizationId;
    public $projectRoles;
    public $projectTicket;
    public $systemRoles;
    public $vendorId;
  }
}

if (!class_exists('cancelSubmission')) {
  class cancelSubmission {
    public $submissionId;
    public $userId;
  }
}

if (!class_exists('cancelSubmissionResponse')) {
  class cancelSubmissionResponse {
    public $return;
  }
}

if (!class_exists('cancelSubmissionWithComment')) {
  class cancelSubmissionWithComment {
    public $submissionId;
    public $comment;
    public $userId;
  }
}

if (!class_exists('cancelSubmissionWithCommentResponse')) {
  class cancelSubmissionWithCommentResponse {
    public $return;
  }
}

if (!class_exists('findByTicket')) {
  class findByTicket {
    public $ticket;
    public $userId;
  }
}

if (!class_exists('findByTicketResponse')) {
  class findByTicketResponse {
    public $return;
  }
}

if (!class_exists('search')) {
  class search {
    public $command;
    public $info;
    public $userId;
  }
}

if (!class_exists('searchResponse')) {
  class searchResponse {
    public $return;
  }
}

if (!class_exists('searchSubmissions')) {
  class searchSubmissions {
    public $submissionSearchRequest;
    public $info;
    public $userId;
  }
}

if (!class_exists('searchSubmissionsResponse')) {
  class searchSubmissionsResponse {
    public $return;
  }
}

if (!class_exists('startSingleBatchSubmission')) {
  class startSingleBatchSubmission {
    public $submissionId;
    public $submissionInfo;
    public $userId;
  }
}

if (!class_exists('startSingleBatchSubmissionResponse')) {
  class startSingleBatchSubmissionResponse {
    public $return;
  }
}

if (!class_exists('startSubmission')) {
  class startSubmission {
    public $submissionId;
    public $submissionInfo;
    public $userId;
  }
}

if (!class_exists('startSubmissionResponse')) {
  class startSubmissionResponse {
    public $return;
  }
}

if (!class_exists('uploadReference')) {
  class uploadReference {
    public $submissionId;
    public $resourceInfo;
    public $data;
    public $userId;
  }
}

if (!class_exists('uploadReferenceResponse')) {
  class uploadReferenceResponse {
    public $return;
  }
}

/**
* SubmissionService2 class
*/
class SubmissionService2 extends SoapClient {
  private static $classmap = array(
    'anonymous0' => 'anonymous0',
    'base64Binary' => 'base64Binary',
    'hexBinary' => 'hexBinary',
    'Notification' => 'Notification',
    'NotificationPriority' => 'NotificationPriority',
    'Announcement' => 'Announcement',
    'ContentMonitorPluginInfo' => 'ContentMonitorPluginInfo',
    'Date' => 'Date',
    'Document' => 'Document',
    'DocumentGroup' => 'DocumentGroup',
    'DocumentInfo' => 'DocumentInfo',
    'DocumentPagedList' => 'DocumentPagedList',
    'DocumentSearchRequest' => 'DocumentSearchRequest',
    'DocumentTicket' => 'DocumentTicket',
    'EntityTypeEnum' => 'EntityTypeEnum',
    'FileFormatProfile' => 'FileFormatProfile',
    'FileFormatProgressData' => 'FileFormatProgressData',
    'FileProgressData' => 'FileProgressData',
    'FuzzyTmStatistics' => 'FuzzyTmStatistics',
    'ItemFolderEnum' => 'ItemFolderEnum',
    'ItemStatusEnum' => 'ItemStatusEnum',
    'Metadata' => 'Metadata',
    'Language' => 'Language',
    'LanguageDirection' => 'LanguageDirection',
    'LanguageDirectionModel' => 'LanguageDirectionModel',
    'PagedListInfo' => 'PagedListInfo',
    'Phase' => 'Phase',
    'Priority' => 'Priority',
    'Project' => 'Project',
    'ProjectInfo' => 'ProjectInfo',
    'ProjectLanguage' => 'ProjectLanguage',
    'RepositoryItem' => 'RepositoryItem',
    'ResourceInfo' => 'ResourceInfo',
    'ResourceType' => 'ResourceType',
    'Submission' => 'Submission',
    'SubmissionInfo' => 'SubmissionInfo',
    'SubmissionPagedList' => 'SubmissionPagedList',
    'SimpleSubmissionSearchModel' => 'SimpleSubmissionSearchModel',
    'SubmissionSearchModelPagedList' => 'SubmissionSearchModelPagedList',
    'SubmissionSearchRequest' => 'SubmissionSearchRequest',
    'Target' => 'Target',
    'TargetInfo' => 'TargetInfo',
    'TargetPagedList' => 'TargetPagedList',
    'TargetSearchRequest' => 'TargetSearchRequest',
    'Task' => 'Task',
    'TmStatistics' => 'TmStatistics',
    'WorkflowDefinition' => 'WorkflowDefinition',
    'UserInfo' => 'UserInfo',
    'TiUserInfo' => 'TiUserInfo',
    'cancelSubmission' => 'cancelSubmission',
    'cancelSubmissionResponse' => 'cancelSubmissionResponse',
    'cancelSubmissionWithComment' => 'cancelSubmissionWithComment',
    'cancelSubmissionWithCommentResponse' => 'cancelSubmissionWithCommentResponse',
    'findByTicket' => 'findByTicket',
    'findByTicketResponse' => 'findByTicketResponse',
    'search' => 'search',
    'searchResponse' => 'searchResponse',
    'searchSubmissions' => 'searchSubmissions',
    'searchSubmissionsResponse' => 'searchSubmissionsResponse',
    'startSingleBatchSubmission' => 'startSingleBatchSubmission',
    'startSingleBatchSubmissionResponse' => 'startSingleBatchSubmissionResponse',
    'startSubmission' => 'startSubmission',
    'startSubmissionResponse' => 'startSubmissionResponse',
    'uploadReference' => 'uploadReference',
    'uploadReferenceResponse' => 'uploadReferenceResponse',
  );

  public function SubmissionService2($wsdl = 'http://localhost:8080/pd4/services/SubmissionService2?wsdl', $options = array()) {
    foreach (self::$classmap as $key => $value) {
      if (!isset($options['classmap'][$key])) {
        $options['classmap'][$key] = $value;
      }
    }

    parent::__construct($wsdl, $options);
  }

  /**
  * Starts submission.
  *
  * @param startSubmission $parameters
  *
  * @return startSubmissionResponse
  */
  public function startSubmission(startSubmission $parameters) {
    return $this->__soapCall('startSubmission', array($parameters), array(
      'uri' => 'http://impl.services2.service.ws.projectdirector.gs4tr.org',
      'soapaction' => '',
    ));
  }

  /**
  * Cancels submission.
  *
  * @param cancelSubmission $parameters
  *
  * @return cancelSubmissionResponse
  */
  public function cancelSubmission(cancelSubmission $parameters) {
    return $this->__soapCall('cancelSubmission', array($parameters), array(
      'uri' => 'http://impl.services2.service.ws.projectdirector.gs4tr.org',
      'soapaction' => '',
    ));
  }

  /**
  * Finds item by ticket.
  *
  * @param findByTicket $parameters
  *
  * @return findByTicketResponse
  */
  public function findByTicket(findByTicket $parameters) {
    return $this->__soapCall('findByTicket', array($parameters), array(
      'uri' => 'http://impl.services2.service.ws.projectdirector.gs4tr.org',
      'soapaction' => '',
    ));
  }

  /**
  * Searches submissions.
  *
  * @param searchSubmissions $parameters
  *
  * @return searchSubmissionsResponse
  */
  public function searchSubmissions(searchSubmissions $parameters) {
    return $this->__soapCall('searchSubmissions', array($parameters), array(
      'uri' => 'http://impl.services2.service.ws.projectdirector.gs4tr.org',
      'soapaction' => '',
    ));
  }

  /**
  * Cancels submission with comment.
  *
  * @param cancelSubmissionWithComment $parameters
  *
  * @return cancelSubmissionWithCommentResponse
  */
  public function cancelSubmissionWithComment(cancelSubmissionWithComment $parameters) {
    return $this->__soapCall('cancelSubmissionWithComment', array($parameters), array(
      'uri' => 'http://impl.services2.service.ws.projectdirector.gs4tr.org',
      'soapaction' => '',
    ));
  }

  /**
  * Starts single batch submission.
  *
  * @param startSingleBatchSubmission $parameters
  *
  * @return startSingleBatchSubmissionResponse
  */
  public function startSingleBatchSubmission(startSingleBatchSubmission $parameters) {
    return $this->__soapCall('startSingleBatchSubmission', array($parameters), array(
      'uri' => 'http://impl.services2.service.ws.projectdirector.gs4tr.org',
      'soapaction' => '',
    ));
  }

  /**
  * Searches by parameters.
  *
  * @param search $parameters
  *
  * @return searchResponse
  */
  public function search(search $parameters) {
    return $this->__soapCall('search', array($parameters), array(
      'uri' => 'http://impl.services2.service.ws.projectdirector.gs4tr.org',
      'soapaction' => '',
    ));
  }

  /**
  * Uploads reference.
  *
  * @param uploadReference $parameters
  *
  * @return uploadReferenceResponse
  */
  public function uploadReference(uploadReference $parameters) {
    return $this->__soapCall('uploadReference', array($parameters), array(
      'uri' => 'http://impl.services2.service.ws.projectdirector.gs4tr.org',
      'soapaction' => '',
    ));
  }
}
