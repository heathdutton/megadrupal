/*
 * @file
 * This file provides the embedable javascript for The Top of the Web (Bedst på nettet)
 *
 * @author Jesper Mathiassen <jm@bellcom.dk>
 */
window.xact_width = 400;
window.xact_height = 400;
window.xact_surveyURL = 'https://www.survey-xact.dk/LinkCollector?key=' + Drupal.settings.bedstpaanettet_key;
window.xact_surveyKey = Drupal.settings.bedstpaanettet_key;
window.xact_probability = 1;
window.xact_baseURL = 'https://www.survey-xact.dk';
window.xact_language = "da";
xact_startPopIn();
