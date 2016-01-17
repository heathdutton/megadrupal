<?php

/**
 * @file
 * API documentation for Search File Attachments module.
 */

/**
 * Change the IPTC tagmarker for information extraction from image files.
 *
 * Modules implementing this hook can controlled, what information can
 * be extracted.
 *
 * @param array $tagmarker
 *   The array with the active markers.
 *     - Key: The Original IPTC Code for the field
 *     - Value: The translated field name.
 *   e.g.: $tagmarker['2#025'] = t('Keywords')
 *
 * Metadata keywords list:
 *   - 1#090 = Iptc.Envelope.CharacterSet
 *   - 2#005 = Iptc.ObjectName
 *   - 2#015 = Iptc.Category
 *   - 2#020 = Iptc.Supplementals
 *   - 2#025 = Iptc.Keywords
 *   - 2#040 = Iptc.SpecialsInstructions
 *   - 2#055 = Iptc.DateCreated
 *   - 2#060 = Iptc.TimeCreated
 *   - 2#062 = Iptc.DigitalCreationDate
 *   - 2#063 = Iptc.DigitalCreationTime
 *   - 2#080 = Iptc.ByLine
 *   - 2#085 = Iptc.ByLineTitle
 *   - 2#090 = Iptc.City
 *   - 2#092 = Iptc.Sublocation
 *   - 2#095 = Iptc.ProvinceState
 *   - 2#100 = Iptc.CountryCode
 *   - 2#101 = Iptc.CountryName
 *   - 2#105 = Iptc.Headline
 *   - 2#110 = Iptc.Credits
 *   - 2#115 = Iptc.Source
 *   - 2#116 = Iptc.Copyright
 *   - 2#118 = Iptc.Contact
 *   - 2#120 = Iptc.Caption
 *   - 2#122 = Iptc.CaptionWriter
 */
function hook_search_file_attachments_exif_tagmarker_alter(&$tagmarker) {
  // Add the keyswords tagmarker.
  $tagmarker['2#025'] = t('Keywords');

  // Remove the copyright tagmarker, so that the copyright information
  // are not used for the file search index.
  unset($tagmarker['2#116']);
}
