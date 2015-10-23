<?php

/**
 * @file
 * Theme file for the document_library_document content type.
 *
 * You can override this template by copying this file into your theme's
 * template folder and name it "document-library-document.tpl.php".
 * Modify as required.
 *
 * Variables passed into Template:
 *
 *      $node - the fully loaded document_library_document node in
 *        case you need to add anything.
 *
 *      $document_thumbnail - an array containing path, width and
 *        height
 *      $document_file] - an array containing path, type and an
 *        array of pre-calculated sizes - B, KB, MB, GB
 *      $document_date - an array containing time, year, month
 *        and day
 *      $document_pages - the number of pages
 *      $document_languages - an imploded list of lanaguages
 *      $document_body - body text
 *      $document_tags - an array of links
 *
 * This file contains all of the default field definitions for the
 * document_library_document content type. Adding or removing any of
 * the default fields will require changes to your theme's version
 * of this template. You will also need to provide variables properly
 * using hook_prepocess_document_library_document() in your theme or
 * by accessing the $node values directly.
 */
?>

<div class='document-library-results-row-container'>
  <div class='document-library-results-row-wrapper'>
    <div class='document-library-results-row'>
      <div class='document-library-left'>
        <div class='document-library-left-thumbnail'><img src='<?php print $document_thumbnail['path']; ?>' width='<?php print $document_thumbnail['width']; ?>'/></div>
        <div class='document-library-left-download'><a href='<?php print $document_file['path']; ?>' class='button' target='_blank'>Download</a></div>
      </div>
      <div class='document-library-right'>
        <div class='document-library-right-title'><?php print check_plain($node->title); ?></div>
        <?php if (isset($document_date)): ?>
          <div class='document-library-right-date'>Date Published: <span><?php print $document_date['year']; ?></span></div>
        <?php endif; ?>
        <div class='document-library-right-info'>
          <?php if (isset($document_pages)): ?><span><?php print $document_pages; ?> pages</span>,<?php endif; ?>
          <?php print $document_file['size']; ?> <?php print $document_file['type']; ?> file
        </div>
        <?php if (isset($document_languages)): ?>
          <div class='document-library-right-language'>Language: <?php print $document_languages; ?></div>
        <?php endif; ?>
        <?php if (isset($document_body)): ?>
          <div class='document-library-right-body'><?php print $document_body; ?></div>
        <?php endif; ?>
        <?php if (isset($document_tags)): ?>
          <div class='document-library-right-tags'>
            <div class='document-library-right-tags-title'>Document Tags</div>
            <?php print implode($document_tags); ?>
          </div>
        <?php endif; ?>
      </div>
    </div>
  </div>
</div>
