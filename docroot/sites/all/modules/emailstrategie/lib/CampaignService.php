<?php

class EmailStrategie_GenerateAuthentification {
  /**
   * @var string
   */
  public $login;

  /**
   * @var string
   */
  public $password;
}

class EmailStrategie_GenerateAuthentificationResponse {
  /**
   * @var EmailStrategie_AuthentificationResult
   */
  public $GenerateAuthentificationResult;
}

class EmailStrategie_AuthentificationResult {
  /**
   * @var EmailStrategie_AuthentificationStatus
   */
  public $status;

  /**
   * @var string
   */
  public $token;
}

class EmailStrategie_AuthentificationStatus {
  const SUCCESS               = 'SUCCESS';
  const BAD_LOGIN_OR_PASSWORD = 'BAD_LOGIN_OR_PASSWORD';
  const TECHNICAL_ERROR       = 'TECHNICAL_ERROR';
  const ACCESS_DENIED         = 'ACCESS_DENIED';
  const CLIENT_BLOCKED        = 'CLIENT_BLOCKED';
  const INVOICE_BLOCKED       = 'INVOICE_BLOCKED';
}

class EmailStrategie_GetIPs {
  /**
   * @var string
   */
  public $token;
}

class EmailStrategie_GetIPsResponse {
  /**
   * @var string[]
   */
  public $GetIPsResult;
}

class EmailStrategie_GetIPsByDomain {
  /**
   * @var string
   */
  public $token;

  /**
   * @var string
   */
  public $domain;
}

class EmailStrategie_GetIPsByDomainResponse {
  /**
   * @var string[]
   */
  public $GetIPsByDomainResult;
}

class EmailStrategie_GetDomains {
  /**
   * @var string
   */
  public $token;
}

class EmailStrategie_GetDomainsResponse {
  /**
   * @var string[]
   */
  public $GetDomainsResult;
}

class EmailStrategie_GetHeadersTemplates {
  /**
   * @var string
   */
  public $token;
}

class EmailStrategie_GetHeadersTemplatesResponse {
  /**
   * @var EmailStrategie_HeaderTemplate[]
   */
  public $GetHeadersTemplatesResult;
}

class EmailStrategie_HeaderTemplate {
  /**
   * @var int
   */
  public $headerID;

  /**
   * @var string
   */
  public $name;
}

class EmailStrategie_GetFootersTemplates {
  /**
   * @var string
   */
  public $token;
}

class EmailStrategie_GetFootersTemplatesResponse {
  /**
   * @var EmailStrategie_FooterTemplate[]
   */
  public $GetFootersTemplatesResult;
}

class EmailStrategie_FooterTemplate {
  /**
   * @var int
   */
  public $footerID;

  /**
   * @var string
   */
  public $name;
}

class EmailStrategie_GetUnsubscribesTemplates {
  /**
   * @var string
   */
  public $token;
}

class EmailStrategie_GetUnsubscribesTemplatesResponse {
  /**
   * @var EmailStrategie_UnsubscribeTemplate[]
   */
  public $GetUnsubscribesTemplatesResult;
}

class EmailStrategie_UnsubscribeTemplate {
  /**
   * @var int
   */
  public $unsubscribeTemplateID;

  /**
   * @var string
   */
  public $name;
}

class EmailStrategie_GetUnsubscribeList {
  /**
   * @var string
   */
  public $token;
}

class EmailStrategie_GetUnsubscribeListResponse {
  /**
   * @var EmailStrategie_UnsubscribeList[]
   */
  public $GetUnsubscribeListResult;
}

class EmailStrategie_UnsubscribeList {
  /**
   * @var string
   */
  public $name;

  /**
   * @var int
   */
  public $unsubscribeListID;
}

class EmailStrategie_CreateUnsubscribeList {
  /**
   * @var string
   */
  public $token;

  /**
   * @var string
   */
  public $unsubscribeListName;
}

class EmailStrategie_CreateUnsubscribeListResponse {
  /**
   * @var EmailStrategie_UnsubscribeList
   */
  public $CreateUnsubscribeListResult;
}

class EmailStrategie_GetRankingFolders {
  /**
   * @var string
   */
  public $token;
}

class EmailStrategie_GetRankingFoldersResponse {
  /**
   * @var EmailStrategie_RankingFolder
   */
  public $GetRankingFoldersResult;
}

class EmailStrategie_RankingFolder {
  /**
   * @var EmailStrategie_RankingFolder
   */
  public $folder;

  /**
   * @var int
   */
  public $rankingFolderID;

  /**
   * @var string
   */
  public $rankingFolderName;
}

class EmailStrategie_GetGlobalTagQuery {
  /**
   * @var string
   */
  public $token;
}

class EmailStrategie_GetGlobalTagQueryResponse {
  /**
   * @var EmailStrategie_GlobalTagQuery[]
   */
  public $GetGlobalTagQueryResult;
}

class EmailStrategie_GlobalTagQuery {
  /**
   * @var int
   */
  public $globalTagQueryID;

  /**
   * @var string
   */
  public $name;
}

class EmailStrategie_GetResponsivTemplates {
  /**
   * @var string
   */
  public $token;
}

class EmailStrategie_GetResponsivTemplatesResponse {
  /**
   * @var EmailStrategie_ResponsivTemplate[]
   */
  public $GetResponsivTemplatesResult;
}

class EmailStrategie_ResponsivTemplate {
  /**
   * @var int
   */
  public $responsivID;

  /**
   * @var string
   */
  public $name;
}

class EmailStrategie_CreateCampaign {
  /**
   * @var string
   */
  public $token;

  /**
   * @var EmailStrategie_Campaign
   */
  public $campaign;
}

class EmailStrategie_Campaign {
  /**
   * @var string
   */
  public $name;

  /**
   * @var string
   */
  public $fromName;

  /**
   * @var string
   */
  public $comments;

  /**
   * @var string
   */
  public $reference;

  /**
   * @var int
   */
  public $rankingFolderID;

  /**
   * @var int
   */
  public $unsubscribeListID;

  /**
   * @var int
   */
  public $exclusionListID;

  /**
   * @var EmailStrategie_ExcludeHardNpai
   */
  public $excludeHardNpai;

  /**
   * @var boolean
   */
  public $isMassRecipients;

  /**
   * @var EmailStrategie_EncodingCampaign
   */
  public $encoding;

  /**
   * @var EmailStrategie_SchedulerSend
   */
  public $scheduler;

  /**
   * @var EmailStrategie_FileOption
   */
  public $fileOption;
}

class EmailStrategie_ExcludeHardNpai {
  /**
   * @var int
   */
  public $number;

  /**
   * @var EmailStrategie_ExcludeHardNpaiType
   */
  public $type;
}

class EmailStrategie_ExcludeHardNpaiType {
  const CAMPAIGN = 'CAMPAIGN';
  const WEEK     = 'WEEK';
  const MONTH    = 'MONTH';
}

class EmailStrategie_EncodingCampaign {
  const UTF_8      = 'UTF_8';
  const ISO_8859_1 = 'ISO_8859_1';
  const US_ASCII   = 'US_ASCII';
}

class EmailStrategie_SchedulerSend {
}

class EmailStrategie_Clocked extends EmailStrategie_SchedulerSend {
  /**
   * @var EmailStrategie_GMT
   */
  public $gmt;

  /**
   * @var int
   */
  public $shardingSize;

  /**
   * @var int
   */
  public $shardingHourBegin;

  /**
   * @var int
   */
  public $shardingHourEnd;

  /**
   * @var string[]
   */
  public $shardingDays;
}

class EmailStrategie_GMT {
  const NO_GMT   = 'NO_GMT';
  const _DEFAULT = 'DEFAULT';
  const MORE_1   = 'MORE_1';
  const MORE_2   = 'MORE_2';
  const MORE_3   = 'MORE_3';
  const MORE_4   = 'MORE_4';
  const MORE_5   = 'MORE_5';
  const MORE_6   = 'MORE_6';
  const MORE_7   = 'MORE_7';
  const MORE_8   = 'MORE_8';
  const MORE_9   = 'MORE_9';
  const MORE_10  = 'MORE_10';
  const MORE_11  = 'MORE_11';
  const MORE_12  = 'MORE_12';
  const MORE_13  = 'MORE_13';
  const LESS_1   = 'LESS_1';
  const LESS_2   = 'LESS_2';
  const LESS_3   = 'LESS_3';
  const LESS_4   = 'LESS_4';
  const LESS_5   = 'LESS_5';
  const LESS_6   = 'LESS_6';
  const LESS_7   = 'LESS_7';
  const LESS_8   = 'LESS_8';
  const LESS_9   = 'LESS_9';
  const LESS_10  = 'LESS_10';
}

class EmailStrategie_ClockedContinuously extends EmailStrategie_SchedulerSend {
  /**
   * @var EmailStrategie_GMT
   */
  public $gmt;

  /**
   * @var int
   */
  public $shardingSize;

  /**
   * @var int
   */
  public $shardingHourBegin;

  /**
   * @var string[]
   */
  public $shardingDays;
}

class EmailStrategie_Scheduled {
  /**
   * @var EmailStrategie_GMT
   */
  public $gmt;

  /**
   * @var string
   */
  public $executionDate;

  /**
   * @var int
   */
  public $shardingHourBegin;
}

class EmailStrategie_Immediate extends EmailStrategie_SchedulerSend {
}

class EmailStrategie_FileOption {
  const NONE                                        = 'NONE';
  const LESSOR_FILE_DISPLAY_ALL_FIELDS              = 'LESSOR_FILE_DISPLAY_ALL_FIELDS';
  const LESSOR_FILE_DISPLAY_ALL_FIELDS_EXCEPT_EMAIL = 'LESSOR_FILE_DISPLAY_ALL_FIELDS_EXCEPT_EMAIL';
  const LESSOR_FILE_HIDDEN_ALL_FIELDS               = 'LESSOR_FILE_HIDDEN_ALL_FIELDS';
  const RENTAL_FILE_DISPLAY_ALL_FIELDS              = 'RENTAL_FILE_DISPLAY_ALL_FIELDS';
  const RENTAL_FILE_DISPLAY_ALL_FIELDS_EXCEPT_EMAIL = 'RENTAL_FILE_DISPLAY_ALL_FIELDS_EXCEPT_EMAIL';
  const RENTAL_FILE_HIDDEN_ALL_FIELDS               = 'RENTAL_FILE_HIDDEN_ALL_FIELDS';
}

class EmailStrategie_CampaignSms extends EmailStrategie_Campaign {
  /**
   * @var string
   */
  public $message;

  /**
   * @var EmailStrategie_TestSmsBAT[]
   */
  public $testBATs;

  /**
   * @var EmailStrategie_ResendSmsStats
   */
  public $resend;
}

class EmailStrategie_TestSmsBAT {
  /**
   * @var boolean
   */
  public $isTest;

  /**
   * @var boolean
   */
  public $isConfirmation;

  /**
   * @var string
   */
  public $email;

  /**
   * @var string
   */
  public $phone;
}

class EmailStrategie_ResendSmsStats extends EmailStrategie_ResendStats {
}

class EmailStrategie_ResendStats {
  /**
   * @var boolean
   */
  public $doCompleteFile;

  /**
   * @var boolean
   */
  public $doDuplicates;

  /**
   * @var boolean
   */
  public $doBounces;

  /**
   * @var boolean
   */
  public $doUnsubscribers;

  /**
   * @var boolean
   */
  public $doIncorrectSyntax;

  /**
   * @var boolean
   */
  public $isFileUnique;

  /**
   * @var EmailStrategie_ResendStatsRecipient
   */
  public $recipient;
}

class EmailStrategie_ResendStatsRecipient {
}

class EmailStrategie_ResendStatsRecipientEmail extends EmailStrategie_ResendStatsRecipient {
  /**
   * @var string[]
   */
  public $emails;
}

class EmailStrategie_ResendStatsRecipientFtp extends EmailStrategie_ResendStatsRecipient {
  /**
   * @var string
   */
  public $server;

  /**
   * @var int
   */
  public $port;

  /**
   * @var string
   */
  public $login;

  /**
   * @var string
   */
  public $password;
}

class EmailStrategie_ResendEmailStats extends EmailStrategie_ResendStats {
  /**
   * @var boolean
   */
  public $doClickers;

  /**
   * @var boolean
   */
  public $doOpeners;

  /**
   * @var boolean
   */
  public $doNotOpeners;

  /**
   * @var boolean
   */
  public $doNotClickers;

  /**
   * @var boolean
   */
  public $doExcludePlatform;

  /**
   * @var boolean
   */
  public $doExcludeUser;

  /**
   * @var EmailStrategie_ResendStatsFrequency
   */
  public $frequency;
}

class EmailStrategie_ResendStatsFrequency {
}

class EmailStrategie_ResendStatsDailyFrequency extends EmailStrategie_ResendStatsFrequency {
  /**
   * @var int
   */
  public $eachHour;

  /**
   * @var int
   */
  public $duringNbDays;
}

class EmailStrategie_ResendStatsUniqueFrequency extends EmailStrategie_ResendStatsFrequency {
  /**
   * @var int
   */
  public $nbDaysAfterSending;
}

class EmailStrategie_CampaignEmail extends EmailStrategie_Campaign {
  /**
   * @var EmailStrategie_Subject
   */
  public $subject;

  /**
   * @var string
   */
  public $fromEmailAddress;

  /**
   * @var string
   */
  public $replyEmailAddress;

  /**
   * @var string
   */
  public $codeStats;

  /**
   * @var string
   */
  public $domain;

  /**
   * @var EmailStrategie_Message
   */
  public $message;

  /**
   * @var string[]
   */
  public $ips;

  /**
   * @var EmailStrategie_TestEmailBAT[]
   */
  public $testBATs;

  /**
   * @var EmailStrategie_ResendEmailStats
   */
  public $resend;

  /**
   * @var EmailStrategie_ShowModuleStats
   */
  public $showModuleStats;

  /**
   * @var EmailStrategie_GoogleAnalytics
   */
  public $googleAnalytics;

  /**
   * @var int[]
   */
  public $globalTagQueryID;
}

class EmailStrategie_Subject {
  /**
   * @var EmailStrategie_Priority
   */
  public $priority;
}

class EmailStrategie_Priority {
  const NORMAL = 'NORMAL';
  const HIGH   = 'HIGH';
  const LOW    = 'LOW';
}

class EmailStrategie_SubjectSplitTest extends EmailStrategie_Subject {
  /**
   * @var int
   */
  public $splitPercentage;

  /**
   * @var string[]
   */
  public $subjectNames;
}

class EmailStrategie_SubjectStandard extends EmailStrategie_Subject {
  /**
   * @var string
   */
  public $subjectName;
}

class EmailStrategie_Message {
  /**
   * @var string
   */
  public $contentHtml;

  /**
   * @var int
   */
  public $responsivID;

  /**
   * @var string
   */
  public $contentText;

  /**
   * @var boolean
   */
  public $embedImages;

  /**
   * @var boolean
   */
  public $doSpamAssassin;

  /**
   * @var boolean
   */
  public $doCheckUrlLinks;

  /**
   * @var boolean
   */
  public $doScreenShot;

  /**
   * @var EmailStrategie_Image[]
   */
  public $images;

  /**
   * @var EmailStrategie_Attachment[]
   */
  public $attachments;

  /**
   * @var EmailStrategie_Unsubscribe
   */
  public $unsubscribe;

  /**
   * @var EmailStrategie_MirrorPage
   */
  public $mirrorPage;

  /**
   * @var EmailStrategie_AntiSpamAction
   */
  public $antiSpamAction;

  /**
   * @var EmailStrategie_Header
   */
  public $header;

  /**
   * @var EmailStrategie_Footer
   */
  public $footer;

  /**
   * @var EmailStrategie_SocialNetwork
   */
  public $socialNetwork;
}

class EmailStrategie_Image {
  /**
   * @var string
   */
  public $fileName;

  /**
   * @var string
   */
  public $bytes;
}

class EmailStrategie_Attachment {
  /**
   * @var string
   */
  public $fileName;

  /**
   * @var string
   */
  public $bytes;
}

class EmailStrategie_Unsubscribe {
  /**
   * @var EmailStrategie_HorizontalAlign
   */
  public $horizontalAlign;

  /**
   * @var EmailStrategie_VerticalAlign
   */
  public $verticalAlign;

  /**
   * @var EmailStrategie_UnsubscribeText
   */
  public $unsubscribeText;

  /**
   * @var EmailStrategie_UnsubscribeLink
   */
  public $unsubscribeLink;

  /**
   * @var string
   */
  public $hexBackgroundColor;

  /**
   * @var EmailStrategie_LanguageCampaign[]
   */
  public $languages;

  /**
   * @var int
   */
  public $unsubscribeTemplateID;
}

class EmailStrategie_HorizontalAlign {
  const CENTER = 'CENTER';
  const LEFT   = 'LEFT';
  const RIGHT  = 'RIGHT';
}

class EmailStrategie_VerticalAlign {
  const TOP    = 'TOP';
  const BOTTOM = 'BOTTOM';
}

class EmailStrategie_UnsubscribeText {
}

class EmailStrategie_MessageTextLink {
  /**
   * @var EmailStrategie_FontFamily
   */
  public $fontFamily;

  /**
   * @var EmailStrategie_FontSize
   */
  public $fontSize;

  /**
   * @var string
   */
  public $hexColor;

  /**
   * @var string
   */
  public $text;
}

class EmailStrategie_FontFamily {
  const ARIAL           = 'ARIAL';
  const TIMES_NEW_ROMAN = 'TIMES_NEW_ROMAN';
  const COURRIER        = 'COURRIER';
  const GEORGIA         = 'GEORGIA';
  const VERDANA         = 'VERDANA';
  const GENAVA          = 'GENAVA';
}

class EmailStrategie_FontSize {
  const SIZE_10 = 'SIZE_10';
  const SIZE_8  = 'SIZE_8';
  const SIZE_9  = 'SIZE_9';
  const SIZE_11 = 'SIZE_11';
  const SIZE_12 = 'SIZE_12';
  const SIZE_13 = 'SIZE_13';
  const SIZE_14 = 'SIZE_14';
  const SIZE_15 = 'SIZE_15';
  const SIZE_16 = 'SIZE_16';
}

class EmailStrategie_MirrorPageLink {
}

class EmailStrategie_MirrorPageText {
}

class EmailStrategie_UnsubscribeLink {
}

class EmailStrategie_LanguageCampaign {
  const FRENCH     = 'FRENCH';
  const ENGLISH    = 'ENGLISH';
  const ITALIAN    = 'ITALIAN';
  const SPANISH    = 'SPANISH';
  const PORTUGUESE = 'PORTUGUESE';
  const GERMAN     = 'GERMAN';
  const ROMANIAN   = 'ROMANIAN';
}

class EmailStrategie_UnsubscribeDefault extends EmailStrategie_Unsubscribe {
}

class EmailStrategie_UnsubscribeWithDefaultReasons extends EmailStrategie_Unsubscribe {
}

class EmailStrategie_UnsubscribeWithCustomizedReasons extends EmailStrategie_Unsubscribe {
}

class EmailStrategie_MirrorPage {
  /**
   * @var EmailStrategie_HorizontalAlign
   */
  public $horizontalAlign;

  /**
   * @var EmailStrategie_MirrorPageText
   */
  public $mirrorPageText;

  /**
   * @var EmailStrategie_MirrorPageLink
   */
  public $mirrorPageLink;

  /**
   * @var string
   */
  public $hexBackgroundColor;
}

class EmailStrategie_AntiSpamAction {
  /**
   * @var EmailStrategie_LanguageCampaign
   */
  public $language;
}

class EmailStrategie_Header {
  /**
   * @var int
   */
  public $headerID;

  /**
   * @var EmailStrategie_HorizontalAlign
   */
  public $horizontalAlign;
}

class EmailStrategie_Footer {
  /**
   * @var int
   */
  public $footerID;

  /**
   * @var EmailStrategie_HorizontalAlign
   */
  public $horizontalAlign;
}

class EmailStrategie_SocialNetwork {
  /**
   * @var EmailStrategie_VerticalAlign
   */
  public $verticalAlign;

  /**
   * @var EmailStrategie_HorizontalAlign
   */
  public $horizontalAlign;

  /**
   * @var EmailStrategie_FontFamily
   */
  public $fontFamily;

  /**
   * @var EmailStrategie_FontSize
   */
  public $fontSize;

  /**
   * @var EmailStrategie_SocialNetworkType[]
   */
  public $socialNetworkTypes;

  /**
   * @var string
   */
  public $shareText;

  /**
   * @var string
   */
  public $hexColor;

  /**
   * @var string
   */
  public $shareUrl;

  /**
   * @var EmailStrategie_FacebookShareContent
   */
  public $facebookShareContent;
}

class EmailStrategie_SocialNetworkType {
  const FACEBOOK    = 'FACEBOOK';
  const TWITTER     = 'TWITTER';
  const GOOGLE_PLUS = 'GOOGLE_PLUS';
  const VIADEO      = 'VIADEO';
  const LINKEDIN    = 'LINKEDIN';
}

class EmailStrategie_FacebookShareContent {
  /**
   * @var string
   */
  public $message;

  /**
   * @var string
   */
  public $title;

  /**
   * @var string
   */
  public $subtitle;

  /**
   * @var string
   */
  public $description;

  /**
   * @var string
   */
  public $shareLinkLabel;
}

class EmailStrategie_TestEmailBAT {
  /**
   * @var boolean
   */
  public $isTest;

  /**
   * @var boolean
   */
  public $isConfirmation;

  /**
   * @var boolean
   */
  public $isValidation;

  /**
   * @var boolean
   */
  public $isSpy;

  /**
   * @var string
   */
  public $email;
}

class EmailStrategie_ShowModuleStats {
  /**
   * @var boolean
   */
  public $enableSynthese;

  /**
   * @var boolean
   */
  public $enableEvolutions;

  /**
   * @var boolean
   */
  public $enableLinkStats;

  /**
   * @var boolean
   */
  public $enableFile;

  /**
   * @var boolean
   */
  public $enableDelivrability;

  /**
   * @var boolean
   */
  public $enableFAIWebMails;

  /**
   * @var boolean
   */
  public $enableOS;

  /**
   * @var boolean
   */
  public $enableMap;

  /**
   * @var boolean
   */
  public $enableAccountClient;

  /**
   * @var boolean
   */
  public $enableProfiles;

  /**
   * @var boolean
   */
  public $enableSources;

  /**
   * @var boolean
   */
  public $enableDownloads;

  /**
   * @var boolean
   */
  public $enableCompare;

  /**
   * @var boolean
   */
  public $enableSocialNetworks;
}

class EmailStrategie_GoogleAnalytics {
  /**
   * @var string
   */
  public $utmCampaign;

  /**
   * @var string
   */
  public $utmSource;

  /**
   * @var string
   */
  public $utmMedium;
}

class EmailStrategie_CampaignSmsVoice extends EmailStrategie_Campaign {
  /**
   * @var string
   */
  public $message;

  /**
   * @var EmailStrategie_TestSmsBAT[]
   */
  public $testBATs;

  /**
   * @var EmailStrategie_ResendSmsStats
   */
  public $resend;

  /**
   * @var EmailStrategie_VoiceLanguage
   */
  public $lang;
}

class EmailStrategie_VoiceLanguage {
  const fr_FR = 'fr_FR';
  const en_GB = 'en_GB';
  const es_ES = 'es_ES';
  const de_DE = 'de_DE';
}

class EmailStrategie_CreateCampaignResponse {
  /**
   * @var EmailStrategie_CampaignResult
   */
  public $CreateCampaignResult;
}

class EmailStrategie_CampaignResult {
  /**
   * @var int
   */
  public $campaignID;

  /**
   * @var EmailStrategie_CampaignResultStatus
   */
  public $status;

  /**
   * @var string
   */
  public $errorContent;
}

class EmailStrategie_CampaignResultStatus {
  const SUCCESS         = 'SUCCESS';
  const ERROR           = 'ERROR';
  const TECHNICAL_ERROR = 'TECHNICAL_ERROR';
  const BAD_TOKEN       = 'BAD_TOKEN';
}

class EmailStrategie_PauseCampaign {
  /**
   * @var string
   */
  public $token;

  /**
   * @var int
   */
  public $campaignID;
}

class EmailStrategie_PauseCampaignResponse {
  /**
   * @var EmailStrategie_PauseResultStatus
   */
  public $PauseCampaignResult;
}

class EmailStrategie_PauseResultStatus {
  const SUCCESS         = 'SUCCESS';
  const FAILED          = 'FAILED';
  const BAD_TOKEN       = 'BAD_TOKEN';
  const BAD_CAMPAIGN    = 'BAD_CAMPAIGN';
  const TECHNICAL_ERROR = 'TECHNICAL_ERROR';
}

class EmailStrategie_RestartPauseCampaign {
  /**
   * @var string
   */
  public $token;

  /**
   * @var int
   */
  public $campaignID;
}

class EmailStrategie_RestartPauseCampaignResponse {
  /**
   * @var EmailStrategie_RestartResultStatus
   */
  public $RestartPauseCampaignResult;
}

class EmailStrategie_RestartResultStatus {
  const SUCCESS         = 'SUCCESS';
  const FAILED          = 'FAILED';
  const BAD_TOKEN       = 'BAD_TOKEN';
  const BAD_CAMPAIGN    = 'BAD_CAMPAIGN';
  const TECHNICAL_ERROR = 'TECHNICAL_ERROR';
}

class EmailStrategie_StopCampaign {
  /**
   * @var string
   */
  public $token;

  /**
   * @var int
   */
  public $campaignID;
}

class EmailStrategie_StopCampaignResponse {
  /**
   * @var EmailStrategie_StopResultStatus
   */
  public $StopCampaignResult;
}

class EmailStrategie_StopResultStatus {
  const SUCCESS         = 'SUCCESS';
  const FAILED          = 'FAILED';
  const BAD_TOKEN       = 'BAD_TOKEN';
  const BAD_CAMPAIGN    = 'BAD_CAMPAIGN';
  const TECHNICAL_ERROR = 'TECHNICAL_ERROR';
}

class EmailStrategie_CreateUploadRecipients {
  /**
   * @var string
   */
  public $token;

  /**
   * @var int
   */
  public $campaignID;

  /**
   * @var string[]
   */
  public $datas;
}

class EmailStrategie_CreateUploadRecipientsResponse {
  /**
   * @var EmailStrategie_UploadRecipientsStatusCode
   */
  public $CreateUploadRecipientsResult;
}

class EmailStrategie_UploadRecipientsStatusCode {
  const SUCCESS            = 'SUCCESS';
  const BAD_TOKEN          = 'BAD_TOKEN';
  const BAD_CAMPAIGN       = 'BAD_CAMPAIGN';
  const NO_RECIPIENTS      = 'NO_RECIPIENTS';
  const TECHNICAL_ERROR    = 'TECHNICAL_ERROR';
  const ALREADY_SENT       = 'ALREADY_SENT';
  const ALREADY_RECIPIENTS = 'ALREADY_RECIPIENTS';
}

class EmailStrategie_StartUploadRecipients {
  /**
   * @var string
   */
  public $token;

  /**
   * @var int
   */
  public $campaignID;

  /**
   * @var EmailStrategie_Recipients
   */
  public $recipients;
}

class EmailStrategie_Recipients {
  /**
   * @var int
   */
  public $sendingColumnIndex;

  /**
   * @var boolean
   */
  public $isCorrectSyntax;

  /**
   * @var boolean
   */
  public $isRemoveDouble;
}

class EmailStrategie_RecipientsStream extends EmailStrategie_Recipients {
}

class EmailStrategie_RecipientsRetargetingStream extends EmailStrategie_Recipients {
  /**
   * @var string
   */
  public $retargetingBddName;

  /**
   * @var string
   */
  public $retargetingFieldName;
}

class EmailStrategie_RecipientsFile extends EmailStrategie_Recipients {
  /**
   * @var string
   */
  public $fileUrl;
}

class EmailStrategie_RecipientsRetargetingFile extends EmailStrategie_Recipients {
  /**
   * @var string
   */
  public $fileUrl;

  /**
   * @var string
   */
  public $retargetingBddName;

  /**
   * @var string
   */
  public $retargetingFieldName;
}

class EmailStrategie_RecipientsBDD extends EmailStrategie_Recipients {
  /**
   * @var int
   */
  public $segmentID;
}

class EmailStrategie_StartUploadRecipientsResponse {
  /**
   * @var EmailStrategie_StartUploadRecipientsStatus
   */
  public $StartUploadRecipientsResult;
}

class EmailStrategie_StartUploadRecipientsStatus {
  /**
   * @var EmailStrategie_UploadRecipientsStatusCode
   */
  public $status;

  /**
   * @var int
   */
  public $uploadOrderID;
}

class EmailStrategie_IsUploadRecipientsFinished {
  /**
   * @var string
   */
  public $token;

  /**
   * @var int
   */
  public $uploadOrderID;
}

class EmailStrategie_IsUploadRecipientsFinishedResponse {
  /**
   * @var EmailStrategie_IsUploadRecipientsFinishedStatus
   */
  public $IsUploadRecipientsFinishedResult;
}

class EmailStrategie_IsUploadRecipientsFinishedStatus {
  const FINISHED        = 'FINISHED';
  const RUNNING         = 'RUNNING';
  const NOT_EXIST       = 'NOT_EXIST';
  const BAD_TOKEN       = 'BAD_TOKEN';
  const TECHNICAL_ERROR = 'TECHNICAL_ERROR';
}

class EmailStrategie_SendCampaign {
  /**
   * @var string
   */
  public $token;

  /**
   * @var int
   */
  public $campaignID;

  /**
   * @var EmailStrategie_SendCampaignType
   */
  public $sendCampaignType;
}

class EmailStrategie_SendCampaignType {
  const TEST_BAT_SENDING = 'TEST_BAT_SENDING';
  const REAL_SENDING     = 'REAL_SENDING';
}

class EmailStrategie_SendCampaignResponse {
  /**
   * @var EmailStrategie_SendCampaignStatus
   */
  public $SendCampaignResult;
}

class EmailStrategie_SendCampaignStatus {
  const SUCCESS            = 'SUCCESS';
  const BAD_TOKEN          = 'BAD_TOKEN';
  const BAD_CAMPAIGN       = 'BAD_CAMPAIGN';
  const NO_RECIPIENTS      = 'NO_RECIPIENTS';
  const TECHNICAL_ERROR    = 'TECHNICAL_ERROR';
  const ALREADY_SENT       = 'ALREADY_SENT';
  const MISSING_VALIDATION = 'MISSING_VALIDATION';
}

class EmailStrategie_SendUnitCampaign {
  /**
   * @var string
   */
  public $token;

  /**
   * @var int
   */
  public $campaignID;

  /**
   * @var string
   */
  public $emailOrPhoneNumber;

  /**
   * @var string[]
   */
  public $customFieldHeaders;

  /**
   * @var string[]
   */
  public $customFieldValues;
}

class EmailStrategie_SendUnitCampaignResponse {
  /**
   * @var EmailStrategie_SendUnitCampaignStatus
   */
  public $SendUnitCampaignResult;
}

class EmailStrategie_SendUnitCampaignStatus {
  const SUCCESS         = 'SUCCESS';
  const BAD_TOKEN       = 'BAD_TOKEN';
  const BAD_CAMPAIGN    = 'BAD_CAMPAIGN';
  const NO_RECIPIENTS   = 'NO_RECIPIENTS';
  const TECHNICAL_ERROR = 'TECHNICAL_ERROR';
  const ERROR           = 'ERROR';
}

class EmailStrategie_SendUnitCampaignToRecipient {
  /**
   * @var string
   */
  public $token;

  /**
   * @var int
   */
  public $campaignID;

  /**
   * @var string
   */
  public $emailOrPhoneNumber;

  /**
   * @var EmailStrategie_OptionalAttribute
   */
  public $option;
}

class EmailStrategie_OptionalAttribute {
  /**
   * @var string[]
   */
  public $customFieldHeaders;

  /**
   * @var string[]
   */
  public $customFieldValues;

  /**
   * @var string
   */
  public $contentHtml;

  /**
   * @var string
   */
  public $contentText;

  /**
   * @var string
   */
  public $subjectName;

  /**
   * @var string
   */
  public $fromName;

  /**
   * @var string
   */
  public $fromEmailAddress;

  /**
   * @var string
   */
  public $replyEmailAddress;

  /**
   * @var EmailStrategie_Attachment[]
   */
  public $attachments;
}

class EmailStrategie_SendUnitCampaignToRecipientResponse {
  /**
   * @var EmailStrategie_SendUnitCampaignStatus
   */
  public $SendUnitCampaignToRecipientResult;
}

class EmailStrategie_GetListFolders {
  /**
   * @var string
   */
  public $token;

  /**
   * @var string
   */
  public $keywords;

  /**
   * @var string
   */
  public $startDate;

  /**
   * @var string
   */
  public $endDate;

  /**
   * @var EmailStrategie_SortOrder
   */
  public $sortOrd;
}

class EmailStrategie_SortOrder {
  const CAMPAIGN_NAME_ASC  = 'CAMPAIGN_NAME_ASC';
  const CAMPAIGN_NAME_DESC = 'CAMPAIGN_NAME_DESC';
  const EXECDATE_ASC       = 'EXECDATE_ASC';
  const EXECDATE_DESC      = 'EXECDATE_DESC';
}

class EmailStrategie_GetListFoldersResponse {
  /**
   * @var EmailStrategie_ListFolder
   */
  public $GetListFoldersResult;
}

class EmailStrategie_ListFolder {
  /**
   * @var int
   */
  public $folderID;

  /**
   * @var string
   */
  public $name;

  /**
   * @var int
   */
  public $nbCampaigns;

  /**
   * @var EmailStrategie_GenericStatus
   */
  public $status;

  /**
   * @var string
   */
  public $errorContent;

  /**
   * @var EmailStrategie_ListFolder[]
   */
  public $childrens;
}

class EmailStrategie_GenericStatus {
  const BAD_TOKEN       = 'BAD_TOKEN';
  const ERROR           = 'ERROR';
  const SUCCESS         = 'SUCCESS';
  const TECHNICAL_ERROR = 'TECHNICAL_ERROR';
}

class EmailStrategie_CreateFolder {
  /**
   * @var string
   */
  public $token;

  /**
   * @var int
   */
  public $parentFolderID;

  /**
   * @var string
   */
  public $name;
}

class EmailStrategie_CreateFolderResponse {
  /**
   * @var EmailStrategie_CreateFolderResult
   */
  public $CreateFolderResult;
}

class EmailStrategie_CreateFolderResult {
  /**
   * @var EmailStrategie_GenericStatus
   */
  public $status;

  /**
   * @var string
   */
  public $errorContent;

  /**
   * @var int
   */
  public $folderID;
}

class EmailStrategie_GetListCampaigns {
  /**
   * @var string
   */
  public $token;

  /**
   * @var int
   */
  public $folderID;

  /**
   * @var string
   */
  public $keywords;

  /**
   * @var string
   */
  public $startDate;

  /**
   * @var string
   */
  public $endDate;

  /**
   * @var int
   */
  public $pageIndex;

  /**
   * @var int
   */
  public $pageSize;

  /**
   * @var EmailStrategie_SortOrder
   */
  public $sortOrd;
}

class EmailStrategie_GetListCampaignsResponse {
  /**
   * @var EmailStrategie_ListCampaign
   */
  public $GetListCampaignsResult;
}

class EmailStrategie_ListCampaign {
  /**
   * @var EmailStrategie_InfoCampaign[]
   */
  public $infoCampaigns;

  /**
   * @var EmailStrategie_GenericStatus
   */
  public $status;

  /**
   * @var string
   */
  public $errorContent;
}

class EmailStrategie_InfoCampaign {
  /**
   * @var int
   */
  public $campaignID;

  /**
   * @var string
   */
  public $name;

  /**
   * @var dateTime
   */
  public $execDate;

  /**
   * @var string
   */
  public $codeStats;

  /**
   * @var string
   */
  public $orderType;

  /**
   * @var int
   */
  public $nbrEmails;

  /**
   * @var int
   */
  public $totalNPAI;

  /**
   * @var double
   */
  public $openPercent;

  /**
   * @var string
   */
  public $imageUrl;
}

/**
 * Class CampaignService
 */
class EmailStrategie_CampaignService extends SoapClient {

  private static $classmap = array(
    'GenerateAuthentification'            => 'EmailStrategie_GenerateAuthentification',
    'GenerateAuthentificationResponse'    => 'EmailStrategie_GenerateAuthentificationResponse',
    'AuthentificationResult'              => 'EmailStrategie_AuthentificationResult',
    'AuthentificationStatus'              => 'EmailStrategie_AuthentificationStatus',
    'GetIPs'                              => 'EmailStrategie_GetIPs',
    'GetIPsResponse'                      => 'EmailStrategie_GetIPsResponse',
    'GetIPsByDomain'                      => 'EmailStrategie_GetIPsByDomain',
    'GetIPsByDomainResponse'              => 'EmailStrategie_GetIPsByDomainResponse',
    'GetDomains'                          => 'EmailStrategie_GetDomains',
    'GetDomainsResponse'                  => 'EmailStrategie_GetDomainsResponse',
    'GetHeadersTemplates'                 => 'EmailStrategie_GetHeadersTemplates',
    'GetHeadersTemplatesResponse'         => 'EmailStrategie_GetHeadersTemplatesResponse',
    'HeaderTemplate'                      => 'EmailStrategie_HeaderTemplate',
    'GetFootersTemplates'                 => 'EmailStrategie_GetFootersTemplates',
    'GetFootersTemplatesResponse'         => 'EmailStrategie_GetFootersTemplatesResponse',
    'FooterTemplate'                      => 'EmailStrategie_FooterTemplate',
    'GetUnsubscribesTemplates'            => 'EmailStrategie_GetUnsubscribesTemplates',
    'GetUnsubscribesTemplatesResponse'    => 'EmailStrategie_GetUnsubscribesTemplatesResponse',
    'UnsubscribeTemplate'                 => 'EmailStrategie_UnsubscribeTemplate',
    'GetUnsubscribeList'                  => 'EmailStrategie_GetUnsubscribeList',
    'GetUnsubscribeListResponse'          => 'EmailStrategie_GetUnsubscribeListResponse',
    'UnsubscribeList'                     => 'EmailStrategie_UnsubscribeList',
    'CreateUnsubscribeList'               => 'EmailStrategie_CreateUnsubscribeList',
    'CreateUnsubscribeListResponse'       => 'EmailStrategie_CreateUnsubscribeListResponse',
    'GetRankingFolders'                   => 'EmailStrategie_GetRankingFolders',
    'GetRankingFoldersResponse'           => 'EmailStrategie_GetRankingFoldersResponse',
    'RankingFolder'                       => 'EmailStrategie_RankingFolder',
    'GetGlobalTagQuery'                   => 'EmailStrategie_GetGlobalTagQuery',
    'GetGlobalTagQueryResponse'           => 'EmailStrategie_GetGlobalTagQueryResponse',
    'GlobalTagQuery'                      => 'EmailStrategie_GlobalTagQuery',
    'GetResponsivTemplates'               => 'EmailStrategie_GetResponsivTemplates',
    'GetResponsivTemplatesResponse'       => 'EmailStrategie_GetResponsivTemplatesResponse',
    'ResponsivTemplate'                   => 'EmailStrategie_ResponsivTemplate',
    'CreateCampaign'                      => 'EmailStrategie_CreateCampaign',
    'Campaign'                            => 'EmailStrategie_Campaign',
    'ExcludeHardNpai'                     => 'EmailStrategie_ExcludeHardNpai',
    'ExcludeHardNpaiType'                 => 'EmailStrategie_ExcludeHardNpaiType',
    'EncodingCampaign'                    => 'EmailStrategie_EncodingCampaign',
    'SchedulerSend'                       => 'EmailStrategie_SchedulerSend',
    'Clocked'                             => 'EmailStrategie_Clocked',
    'GMT'                                 => 'EmailStrategie_GMT',
    'ClockedContinuously'                 => 'EmailStrategie_ClockedContinuously',
    'Scheduled'                           => 'EmailStrategie_Scheduled',
    'Immediate'                           => 'EmailStrategie_Immediate',
    'FileOption'                          => 'EmailStrategie_FileOption',
    'CampaignSms'                         => 'EmailStrategie_CampaignSms',
    'TestSmsBAT'                          => 'EmailStrategie_TestSmsBAT',
    'ResendSmsStats'                      => 'EmailStrategie_ResendSmsStats',
    'ResendStats'                         => 'EmailStrategie_ResendStats',
    'ResendStatsRecipient'                => 'EmailStrategie_ResendStatsRecipient',
    'ResendStatsRecipientEmail'           => 'EmailStrategie_ResendStatsRecipientEmail',
    'ResendStatsRecipientFtp'             => 'EmailStrategie_ResendStatsRecipientFtp',
    'ResendEmailStats'                    => 'EmailStrategie_ResendEmailStats',
    'ResendStatsFrequency'                => 'EmailStrategie_ResendStatsFrequency',
    'ResendStatsDailyFrequency'           => 'EmailStrategie_ResendStatsDailyFrequency',
    'ResendStatsUniqueFrequency'          => 'EmailStrategie_ResendStatsUniqueFrequency',
    'CampaignEmail'                       => 'EmailStrategie_CampaignEmail',
    'Subject'                             => 'EmailStrategie_Subject',
    'Priority'                            => 'EmailStrategie_Priority',
    'SubjectSplitTest'                    => 'EmailStrategie_SubjectSplitTest',
    'SubjectStandard'                     => 'EmailStrategie_SubjectStandard',
    'Message'                             => 'EmailStrategie_Message',
    'Image'                               => 'EmailStrategie_Image',
    'Attachment'                          => 'EmailStrategie_Attachment',
    'Unsubscribe'                         => 'EmailStrategie_Unsubscribe',
    'HorizontalAlign'                     => 'EmailStrategie_HorizontalAlign',
    'VerticalAlign'                       => 'EmailStrategie_VerticalAlign',
    'UnsubscribeText'                     => 'EmailStrategie_UnsubscribeText',
    'MessageTextLink'                     => 'EmailStrategie_MessageTextLink',
    'FontFamily'                          => 'EmailStrategie_FontFamily',
    'FontSize'                            => 'EmailStrategie_FontSize',
    'MirrorPageLink'                      => 'EmailStrategie_MirrorPageLink',
    'MirrorPageText'                      => 'EmailStrategie_MirrorPageText',
    'UnsubscribeLink'                     => 'EmailStrategie_UnsubscribeLink',
    'LanguageCampaign'                    => 'EmailStrategie_LanguageCampaign',
    'UnsubscribeDefault'                  => 'EmailStrategie_UnsubscribeDefault',
    'UnsubscribeWithDefaultReasons'       => 'EmailStrategie_UnsubscribeWithDefaultReasons',
    'UnsubscribeWithCustomizedReasons'    => 'EmailStrategie_UnsubscribeWithCustomizedReasons',
    'MirrorPage'                          => 'EmailStrategie_MirrorPage',
    'AntiSpamAction'                      => 'EmailStrategie_AntiSpamAction',
    'Header'                              => 'EmailStrategie_Header',
    'Footer'                              => 'EmailStrategie_Footer',
    'SocialNetwork'                       => 'EmailStrategie_SocialNetwork',
    'SocialNetworkType'                   => 'EmailStrategie_SocialNetworkType',
    'FacebookShareContent'                => 'EmailStrategie_FacebookShareContent',
    'TestEmailBAT'                        => 'EmailStrategie_TestEmailBAT',
    'ShowModuleStats'                     => 'EmailStrategie_ShowModuleStats',
    'GoogleAnalytics'                     => 'EmailStrategie_GoogleAnalytics',
    'CampaignSmsVoice'                    => 'EmailStrategie_CampaignSmsVoice',
    'VoiceLanguage'                       => 'EmailStrategie_VoiceLanguage',
    'CreateCampaignResponse'              => 'EmailStrategie_CreateCampaignResponse',
    'CampaignResult'                      => 'EmailStrategie_CampaignResult',
    'CampaignResultStatus'                => 'EmailStrategie_CampaignResultStatus',
    'PauseCampaign'                       => 'EmailStrategie_PauseCampaign',
    'PauseCampaignResponse'               => 'EmailStrategie_PauseCampaignResponse',
    'PauseResultStatus'                   => 'EmailStrategie_PauseResultStatus',
    'RestartPauseCampaign'                => 'EmailStrategie_RestartPauseCampaign',
    'RestartPauseCampaignResponse'        => 'EmailStrategie_RestartPauseCampaignResponse',
    'RestartResultStatus'                 => 'EmailStrategie_RestartResultStatus',
    'StopCampaign'                        => 'EmailStrategie_StopCampaign',
    'StopCampaignResponse'                => 'EmailStrategie_StopCampaignResponse',
    'StopResultStatus'                    => 'EmailStrategie_StopResultStatus',
    'CreateUploadRecipients'              => 'EmailStrategie_CreateUploadRecipients',
    'CreateUploadRecipientsResponse'      => 'EmailStrategie_CreateUploadRecipientsResponse',
    'UploadRecipientsStatusCode'          => 'EmailStrategie_UploadRecipientsStatusCode',
    'StartUploadRecipients'               => 'EmailStrategie_StartUploadRecipients',
    'Recipients'                          => 'EmailStrategie_Recipients',
    'RecipientsStream'                    => 'EmailStrategie_RecipientsStream',
    'RecipientsRetargetingStream'         => 'EmailStrategie_RecipientsRetargetingStream',
    'RecipientsFile'                      => 'EmailStrategie_RecipientsFile',
    'RecipientsRetargetingFile'           => 'EmailStrategie_RecipientsRetargetingFile',
    'RecipientsBDD'                       => 'EmailStrategie_RecipientsBDD',
    'StartUploadRecipientsResponse'       => 'EmailStrategie_StartUploadRecipientsResponse',
    'StartUploadRecipientsStatus'         => 'EmailStrategie_StartUploadRecipientsStatus',
    'IsUploadRecipientsFinished'          => 'EmailStrategie_IsUploadRecipientsFinished',
    'IsUploadRecipientsFinishedResponse'  => 'EmailStrategie_IsUploadRecipientsFinishedResponse',
    'IsUploadRecipientsFinishedStatus'    => 'EmailStrategie_IsUploadRecipientsFinishedStatus',
    'SendCampaign'                        => 'EmailStrategie_SendCampaign',
    'SendCampaignType'                    => 'EmailStrategie_SendCampaignType',
    'SendCampaignResponse'                => 'EmailStrategie_SendCampaignResponse',
    'SendCampaignStatus'                  => 'EmailStrategie_SendCampaignStatus',
    'SendUnitCampaign'                    => 'EmailStrategie_SendUnitCampaign',
    'SendUnitCampaignResponse'            => 'EmailStrategie_SendUnitCampaignResponse',
    'SendUnitCampaignStatus'              => 'EmailStrategie_SendUnitCampaignStatus',
    'SendUnitCampaignToRecipient'         => 'EmailStrategie_SendUnitCampaignToRecipient',
    'OptionalAttribute'                   => 'EmailStrategie_OptionalAttribute',
    'SendUnitCampaignToRecipientResponse' => 'EmailStrategie_SendUnitCampaignToRecipientResponse',
    'GetListFolders'                      => 'EmailStrategie_GetListFolders',
    'SortOrder'                           => 'EmailStrategie_SortOrder',
    'GetListFoldersResponse'              => 'EmailStrategie_GetListFoldersResponse',
    'ListFolder'                          => 'EmailStrategie_ListFolder',
    'GenericStatus'                       => 'EmailStrategie_GenericStatus',
    'CreateFolder'                        => 'EmailStrategie_CreateFolder',
    'CreateFolderResponse'                => 'EmailStrategie_CreateFolderResponse',
    'CreateFolderResult'                  => 'EmailStrategie_CreateFolderResult',
    'GetListCampaigns'                    => 'EmailStrategie_GetListCampaigns',
    'GetListCampaignsResponse'            => 'EmailStrategie_GetListCampaignsResponse',
    'ListCampaign'                        => 'EmailStrategie_ListCampaign',
    'InfoCampaign'                        => 'EmailStrategie_InfoCampaign',
  );

  public function __construct(
    $wsdl = 'http://www.wewmanager.com/api/Campaigns/CampaignService.asmx?wsdl',
    $options = array(
      'trace'     => 1,
      'exception' => 1,
    )
  ) {
    foreach (self::$classmap as $key => $value) {
      if (!isset($options['classmap'][$key])) {
        $options['classmap'][$key] = $value;
      }
    }

    parent::SoapClient($wsdl, $options);
  }

  /**
   * Create the token service. The given token expires after 24 hours
   *
   * @param EmailStrategie_GenerateAuthentification $parameters
   *
   * @return EmailStrategie_GenerateAuthentificationResponse
   */
  public function generateAuthentification(EmailStrategie_GenerateAuthentification $parameters) {
    return $this->__soapCall(
      'GenerateAuthentification',
      array($parameters),
      array(
        'uri'        => 'http://www.wewmanager.com/services/campaigns/',
        'soapaction' => ''
      )
    );
  }

  /**
   * Get all the allowed IPs
   *
   * @param EmailStrategie_GetIPs $parameters
   *
   * @return EmailStrategie_GetIPsResponse
   */
  public function getIPs(EmailStrategie_GetIPs $parameters) {
    return $this->__soapCall(
      'GetIPs',
      array($parameters),
      array(
        'uri'        => 'http://www.wewmanager.com/services/campaigns/',
        'soapaction' => ''
      )
    );
  }

  /**
   * Get all the allowed IPs by domain
   *
   * @param EmailStrategie_GetIPsByDomain $parameters
   *
   * @return EmailStrategie_GetIPsByDomainResponse
   */
  public function getIPsByDomain(EmailStrategie_GetIPsByDomain $parameters) {
    return $this->__soapCall(
      'GetIPsByDomain',
      array($parameters),
      array(
        'uri'        => 'http://www.wewmanager.com/services/campaigns/',
        'soapaction' => ''
      )
    );
  }

  /**
   * Get all the allowed domains
   *
   * @param EmailStrategie_GetDomains $parameters
   *
   * @return EmailStrategie_GetDomainsResponse
   */
  public function getDomains(EmailStrategie_GetDomains $parameters) {
    return $this->__soapCall(
      'GetDomains',
      array($parameters),
      array(
        'uri'        => 'http://www.wewmanager.com/services/campaigns/',
        'soapaction' => ''
      )
    );
  }

  /**
   * Get all existing headers templates
   *
   * @param EmailStrategie_GetHeadersTemplates $parameters
   *
   * @return EmailStrategie_GetHeadersTemplatesResponse
   */
  public function getHeadersTemplates(EmailStrategie_GetHeadersTemplates $parameters) {
    return $this->__soapCall(
      'GetHeadersTemplates',
      array($parameters),
      array(
        'uri'        => 'http://www.wewmanager.com/services/campaigns/',
        'soapaction' => ''
      )
    );
  }

  /**
   * Get all existing footers templates
   *
   * @param EmailStrategie_GetFootersTemplates $parameters
   *
   * @return EmailStrategie_GetFootersTemplatesResponse
   */
  public function getFootersTemplates(EmailStrategie_GetFootersTemplates $parameters) {
    return $this->__soapCall(
      'GetFootersTemplates',
      array($parameters),
      array(
        'uri'        => 'http://www.wewmanager.com/services/campaigns/',
        'soapaction' => ''
      )
    );
  }

  /**
   * Get all existing unsubscribe templates
   *
   * @param EmailStrategie_GetUnsubscribesTemplates $parameters
   *
   * @return EmailStrategie_GetUnsubscribesTemplatesResponse
   */
  public function getUnsubscribesTemplates(EmailStrategie_GetUnsubscribesTemplates $parameters) {
    return $this->__soapCall(
      'GetUnsubscribesTemplates',
      array($parameters),
      array(
        'uri'        => 'http://www.wewmanager.com/services/campaigns/',
        'soapaction' => ''
      )
    );
  }

  /**
   * Get all existing unsubscribe lists
   *
   * @param EmailStrategie_GetUnsubscribeList $parameters
   *
   * @return EmailStrategie_GetUnsubscribeListResponse
   */
  public function getUnsubscribeList(EmailStrategie_GetUnsubscribeList $parameters) {
    return $this->__soapCall(
      'GetUnsubscribeList',
      array($parameters),
      array(
        'uri'        => 'http://www.wewmanager.com/services/campaigns/',
        'soapaction' => ''
      )
    );
  }

  /**
   * Create an unsubscribe list
   *
   * @param EmailStrategie_CreateUnsubscribeList $parameters
   *
   * @return EmailStrategie_CreateUnsubscribeListResponse
   */
  public function createUnsubscribeList(EmailStrategie_CreateUnsubscribeList $parameters) {
    return $this->__soapCall(
      'CreateUnsubscribeList',
      array($parameters),
      array(
        'uri'        => 'http://www.wewmanager.com/services/campaigns/',
        'soapaction' => ''
      )
    );
  }

  /**
   * Get all ranking folders
   *
   * @param EmailStrategie_GetRankingFolders $parameters
   *
   * @return EmailStrategie_GetRankingFoldersResponse
   */
  public function getRankingFolders(EmailStrategie_GetRankingFolders $parameters) {
    return $this->__soapCall(
      'GetRankingFolders',
      array($parameters),
      array(
        'uri'        => 'http://www.wewmanager.com/services/campaigns/',
        'soapaction' => ''
      )
    );
  }

  /**
   * Get all global tag and query
   *
   * @param EmailStrategie_GetGlobalTagQuery $parameters
   *
   * @return EmailStrategie_GetGlobalTagQueryResponse
   */
  public function getGlobalTagQuery(EmailStrategie_GetGlobalTagQuery $parameters) {
    return $this->__soapCall(
      'GetGlobalTagQuery',
      array($parameters),
      array(
        'uri'        => 'http://www.wewmanager.com/services/campaigns/',
        'soapaction' => ''
      )
    );
  }

  /**
   * Get all responsiv templates
   *
   * @param EmailStrategie_GetResponsivTemplates $parameters
   *
   * @return EmailStrategie_GetResponsivTemplatesResponse
   */
  public function getResponsivTemplates(EmailStrategie_GetResponsivTemplates $parameters) {
    return $this->__soapCall(
      'GetResponsivTemplates',
      array($parameters),
      array(
        'uri'        => 'http://www.wewmanager.com/services/campaigns/',
        'soapaction' => ''
      )
    );
  }

  /**
   * Create an email or  campaign
   *
   * @param EmailStrategie_CreateCampaign $parameters
   *
   * @return EmailStrategie_CreateCampaignResponse
   */
  public function createCampaign(EmailStrategie_CreateCampaign $parameters) {
    return $this->__soapCall(
      'CreateCampaign',
      array($parameters),
      array(
        'uri'        => 'http://www.wewmanager.com/services/campaigns/',
        'soapaction' => ''
      )
    );
  }

  /**
   * Pause a campaign
   *
   * @param EmailStrategie_PauseCampaign $parameters
   *
   * @return EmailStrategie_PauseCampaignResponse
   */
  public function pauseCampaign(EmailStrategie_PauseCampaign $parameters) {
    return $this->__soapCall(
      'PauseCampaign',
      array($parameters),
      array(
        'uri'        => 'http://www.wewmanager.com/services/campaigns/',
        'soapaction' => ''
      )
    );
  }

  /**
   * Restart a pause campaign
   *
   * @param EmailStrategie_RestartPauseCampaign $parameters
   *
   * @return EmailStrategie_RestartPauseCampaignResponse
   */
  public function restartPauseCampaign(EmailStrategie_RestartPauseCampaign $parameters) {
    return $this->__soapCall(
      'RestartPauseCampaign',
      array($parameters),
      array(
        'uri'        => 'http://www.wewmanager.com/services/campaigns/',
        'soapaction' => ''
      )
    );
  }

  /**
   * Stop a sending campaign
   *
   * @param EmailStrategie_StopCampaign $parameters
   *
   * @return EmailStrategie_StopCampaignResponse
   */
  public function stopCampaign(EmailStrategie_StopCampaign $parameters) {
    return $this->__soapCall(
      'StopCampaign',
      array($parameters),
      array(
        'uri'        => 'http://www.wewmanager.com/services/campaigns/',
        'soapaction' => ''
      )
    );
  }

  /**
   * Create the recipients by stream batch size 1000 for one mass recipients campaign
   *
   * @param EmailStrategie_CreateUploadRecipients $parameters
   *
   * @return EmailStrategie_CreateUploadRecipientsResponse
   */
  public function createUploadRecipients(EmailStrategie_CreateUploadRecipients $parameters) {
    return $this->__soapCall(
      'CreateUploadRecipients',
      array($parameters),
      array(
        'uri'        => 'http://www.wewmanager.com/services/campaigns/',
        'soapaction' => ''
      )
    );
  }

  /**
   * Start the recipients upload for one mass recipients campaign
   *
   * @param EmailStrategie_StartUploadRecipients $parameters
   *
   * @return EmailStrategie_StartUploadRecipientsResponse
   */
  public function startUploadRecipients(EmailStrategie_StartUploadRecipients $parameters) {
    return $this->__soapCall(
      'StartUploadRecipients',
      array($parameters),
      array(
        'uri'        => 'http://www.wewmanager.com/services/campaigns/',
        'soapaction' => ''
      )
    );
  }

  /**
   * Get the status for a mass recipients upload
   *
   * @param EmailStrategie_IsUploadRecipientsFinished $parameters
   *
   * @return EmailStrategie_IsUploadRecipientsFinishedResponse
   */
  public function isUploadRecipientsFinished(EmailStrategie_IsUploadRecipientsFinished $parameters) {
    return $this->__soapCall(
      'IsUploadRecipientsFinished',
      array($parameters),
      array(
        'uri'        => 'http://www.wewmanager.com/services/campaigns/',
        'soapaction' => ''
      )
    );
  }

  /**
   * Send Test BAT or real sending for one mass recipients campaign
   *
   * @param EmailStrategie_SendCampaign $parameters
   *
   * @return EmailStrategie_SendCampaignResponse
   */
  public function sendCampaign(EmailStrategie_SendCampaign $parameters) {
    return $this->__soapCall(
      'SendCampaign',
      array($parameters),
      array(
        'uri'        => 'http://www.wewmanager.com/services/campaigns/',
        'soapaction' => ''
      )
    );
  }

  /**
   * [Deprecated - Use SendUnitCampaignToRecipient instead of SendUnitCampaign] Send to one
   * recipient the campaign's content
   *
   * @param EmailStrategie_SendUnitCampaign $parameters
   *
   * @return EmailStrategie_SendUnitCampaignResponse
   */
  public function sendUnitCampaign(EmailStrategie_SendUnitCampaign $parameters) {
    return $this->__soapCall(
      'SendUnitCampaign',
      array($parameters),
      array(
        'uri'        => 'http://www.wewmanager.com/services/campaigns/',
        'soapaction' => ''
      )
    );
  }

  /**
   * Send to one recipient the campaign's content
   *
   * @param EmailStrategie_SendUnitCampaignToRecipient $parameters
   *
   * @return EmailStrategie_SendUnitCampaignToRecipientResponse
   */
  public function sendUnitCampaignToRecipient(EmailStrategie_SendUnitCampaignToRecipient $parameters) {
    return $this->__soapCall(
      'SendUnitCampaignToRecipient',
      array($parameters),
      array(
        'uri'        => 'http://www.wewmanager.com/services/campaigns/',
        'soapaction' => ''
      )
    );
  }

  /**
   * Retrieve all folders list
   *
   * @param EmailStrategie_GetListFolders $parameters
   *
   * @return EmailStrategie_GetListFoldersResponse
   */
  public function getListFolders(EmailStrategie_GetListFolders $parameters) {
    return $this->__soapCall(
      'GetListFolders',
      array($parameters),
      array(
        'uri'        => 'http://www.wewmanager.com/services/campaigns/',
        'soapaction' => ''
      )
    );
  }

  /**
   * Create a new folder
   *
   * @param EmailStrategie_CreateFolder $parameters
   *
   * @return EmailStrategie_CreateFolderResponse
   */
  public function createFolder(EmailStrategie_CreateFolder $parameters) {
    return $this->__soapCall(
      'CreateFolder',
      array($parameters),
      array(
        'uri'        => 'http://www.wewmanager.com/services/campaigns/',
        'soapaction' => ''
      )
    );
  }

  /**
   * Retrieve campaigns list for a folder
   *
   * @param EmailStrategie_GetListCampaigns $parameters
   *
   * @return EmailStrategie_GetListCampaignsResponse
   */
  public function getListCampaigns(EmailStrategie_GetListCampaigns $parameters) {
    return $this->__soapCall(
      'GetListCampaigns',
      array($parameters),
      array(
        'uri'        => 'http://www.wewmanager.com/services/campaigns/',
        'soapaction' => ''
      )
    );
  }
}
