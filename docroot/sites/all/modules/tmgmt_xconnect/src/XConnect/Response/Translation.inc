<?php
/**
 * @file
 * Wrapper around a single translation file in a response.
 */

/**
 * Wrapper around a single translation file in a response.
 */
class TmgmtXConnectResponseTranslation {
  /**
   * The archive that holds the translations.
   *
   * @var TmgmtXConnectResponseFile
   */
  private $file;

  /**
   * The info about the translation.
   *
   * @var object
   */
  private $info;


  /**
   * Construct by passing the parent archive and the translation file info.
   *
   * @param TmgmtXConnectResponseFile $file
   *   The Archive object.
   * @param object $info
   *   The file information.
   */
  public function __construct(TmgmtXConnectResponseFile $file, $info) {
    $this->file = $file;
    $this->info = $info;
  }

  /**
   * Get the translation type based on the file extension.
   *
   * @return string
   *   The file format.
   */
  public function getFormat() {
    return pathinfo($this->getFileName(), PATHINFO_EXTENSION);
  }

  /**
   * Get the filename of the translation file.
   *
   * @return string
   *   The filename.
   */
  public function getFileName() {
    return $this->info->FileName;
  }

  /**
   * Get the file size of the translation file.
   *
   * @return int
   *   The file size in bytes.
   */
  public function getFileSize() {
    return (int) $this->info->FileSize;
  }

  /**
   * Get the language code of the source for this translation.
   *
   * @return string
   *   The ISO code of the source language.
   */
  public function getSourceLanguageIsoCode() {
    return $this->info->SourceLangIsoCode;
  }

  /**
   * Get the language code of this translation.
   *
   * @return string
   *   The ISO code of the language.
   */
  public function getLanguageIsoCode() {
    return $this->info->TargetLangIsoCode;
  }

  /**
   * Get the full file path within the archive of this translation file.
   *
   * @return string
   *   The file path.
   */
  public function getFilePath() {
    return $this->info->FileReference;
  }

  /**
   * Get the content of the translation file.
   *
   * @return string
   *   The RAW content of the translation file.
   */
  public function getContent() {
    return $this->file->getFileContent($this->getFilePath());
  }

}
