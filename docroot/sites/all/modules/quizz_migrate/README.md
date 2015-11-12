Quizz migrate
====

This module provides migrate scripts to migrate content from quiz 4.x to quizz

1. Backup your site

  Migrate is dangerous, your content maybe lost. To be safe, backup everything.

2. Make sure you are running latest quiz-4.x, schema is updated (ran /update.php)

3. Make sure you copied (manually) all custom fields from quiz node type
  (/admin/structure/types/manage/quiz/fields) to quiz type (/admin/structure/quiz/manage/quiz/fields)
  You can use bundle_copy if you have a lot of fields.

4. Migrate quiz content

  Before run, you should review field mappings at /admin/content/migrate/groups/quiz/quiz
  If there's any issue, please file a new issue at https://www.drupal.org/node/add/project-issue/quizz_migrate

  Run it:

    drush mi quiz
    drush mi quiz_revision

5. Migrate quiz question types

  Enable the question types modules (quizz_cloze, quizz_text, quizz_multichoices, …)

    drush en quizz_cloze quizz_ddlines quizz_matching quizz_multichoice quizz_pool quizz_scale quizz_text quizz_truefalse

  Run question type migration script

    drush migrate-deregister quiz_question
    drush mi quiz_question_type
    drush mi --group=quiz4x_question
    drush mi --group=quiz4x_question_revision

6. Migrate quiz & question relationships

    drush mi quiz_relationship
