<?php
/**
 * @file
 * Returns the HTML for a CM agenda.
 */
?>
<article class="node-<?php print $node->nid; ?> <?php print $classes; ?> cm-agenda clearfix"<?php print $attributes; ?>>
  <div class="cm-agenda-left">
    <div class="cm-agenda-wrapper">
      <?php
        $message_content = render($message);
        if (!empty($message_content)) : ?>
        <?php print $message_content; ?>
      <?php endif; ?>
      <div class="cm-agenda-video">
        <?php
          $video_content = render($video);
          if (!empty($video_content)) : ?>
          <?php print $video_content; ?>
        <?php else: ?>
          <?php print '<div class="alert alert-warning">' . t('No video has been configured') . '</div>'; ?>
        <?php endif; ?>
      </div>
      <div class="cm-agenda-speaker-container">
        <?php if (!empty($speakers)) : ?>
          <?php foreach($speakers as $speaker) : ?>
            <div id="cm-agenda-speaker-<?php print $speaker->media_event_id; ?>" class="cm-agenda-speaker-name"><h2><?php print t('Speaker'); ?></h2><span id="cm-agenda-speaker-name"><?php print $speaker->title; ?></span></div>
          <?php endforeach; ?>
        <?php endif; ?>
        <?php if (!empty($speaker_form)) : ?>
          <?php print render($speaker_form); ?>
        <?php endif; ?>
      </div>
      <?php if (!empty($chapter_info_list)) : ?>
        <div class="cm-agenda-info-container">
          <?php foreach($chapter_info_list as $chapter_info) : ?>
            <div id="cm-agenda-info-<?php print $chapter_info->media_event_id; ?>" class="cm-agenda-info scrollbox">
              <div class="cm-agenda-info-wrapper">
                <h2 class="cm-agenda-info-title"><?php print $chapter_info->title; ?></h2>
                <div class="cm-agenda-info-text"><?php print $chapter_info->text; ?></div>
              </div>
            </div>
          <?php endforeach; ?>
        </div>
      <?php endif; ?>
      <?php if (!empty($settings)) : ?>
        <div class="cm-agenda-settings"><?php print render($settings); ?></div>
      <?php endif; ?>
    </div>
  </div>
  <div class="cm-agenda-right">
    <?php if (!empty($workflow)) : ?>
      <div class="cm-agenda-workflow"><?php print render($workflow); ?></div>
    <?php endif; ?>
    <?php if (empty($chapter_form)) : ?>
      <div class="cm-agenda-tabs">
        <?php if (!empty($chapters)) : ?>
          <div class="cm-agenda-tab cm-agenda-tab-chapters">
            <input class="cm-agenda-tab-radio" type="radio" id="cm-agenda-tab-chapters" name="cm-agenda-tab-group-tabs" checked>
            <label class="cm-agenda-tab-label" for="cm-agenda-tab-chapters"><?php print t('Chapters') ?></label>
            <div class="cm-agenda-tab-panel">
              <div class="cm-agenda-tab-content">
                <div class="cm-agenda-chapter-container">
                  <ul class="cm-agenda-chapter-list scrollbox">
                  <?php
                    $next_speaker = !empty($speakers) ? array_shift($speakers) : NULL;
                    foreach($chapters as $chapter) : ?>
                    <li id="cm-agenda-chapter-<?php print $chapter->media_event_id; ?>" class="cm-agenda-chapter">
                      <?php if (!empty($print_chapter_links)) : ?>
                        <a href="#"
                        data-popcorn-chapter-source="<?php if(!empty($chapter->field_media_event_media_ref[LANGUAGE_NONE][0]['target_id'])) : print $chapter->field_media_event_media_ref[LANGUAGE_NONE][0]['target_id']; endif; ?>"
                        data-popcorn-start="<?php if(!empty($chapter->field_media_event_start[LANGUAGE_NONE][0]['value'])) : print $chapter->field_media_event_start[LANGUAGE_NONE][0]['value'];  endif; ?>"
                        data-popcorn-end="<?php if(!empty($chapter->field_media_event_end[LANGUAGE_NONE][0]['value'])) : print $chapter->field_media_event_end[LANGUAGE_NONE][0]['value'];  endif; ?>"
                        data-popcorn-title="<?php print $chapter->title ?>"
                        data-popcorn-type="cue_field_popcornjs" class="cm-agenda-chapter-spanlink"
                        ><?php print $chapter->title; ?></a>
                      <?php else : ?>
                        <span class="cm-agenda-chapter-spanlink">
                          <?php print $chapter->title; ?>
                        </span>
                      <?php endif; ?>

                      <?php if (!empty($print_chapter_speakers)) :
                        while(!empty($next_speaker) && !empty($next_speaker->field_media_event_end[LANGUAGE_NONE][0]['value']) && $next_speaker->field_media_event_end[LANGUAGE_NONE][0]['value'] < $chapter->field_media_event_start[LANGUAGE_NONE][0]['value']) :
                          $next_speaker = array_shift($speakers);
                        endwhile;
                        $chapter_speakers = array();
                        while(!empty($next_speaker) && !(empty($chapter->field_media_event_end[LANGUAGE_NONE][0]['value'])) && $next_speaker->field_media_event_start[LANGUAGE_NONE][0]['value'] < $chapter->field_media_event_end[LANGUAGE_NONE][0]['value']) :
                          $chapter_speakers[] = $next_speaker;
                          $next_speaker = array_shift($speakers);
                        endwhile;
                        if(!empty($chapter_speakers)) :?>
                          <ul>
                            <?php foreach($chapter_speakers as $speaker) : ?>
                              <li id="cm-agenda-chapter-<?php print $speaker->media_event_id; ?>">
                                <?php if (!empty($print_chapter_links)) : ?>
                                  <a href="#"
                                    data-popcorn-chapter-source="<?php if(!empty($speaker->field_media_event_media_ref[LANGUAGE_NONE][0]['target_id'])) : print $speaker->field_media_event_media_ref[LANGUAGE_NONE][0]['target_id']; endif; ?>"
                                    data-popcorn-start="<?php if(!empty($speaker->field_media_event_start[LANGUAGE_NONE][0]['value'])) : print $speaker->field_media_event_start[LANGUAGE_NONE][0]['value']; endif; ?>"
                                    data-popcorn-end="<?php if(!empty($speaker->field_media_event_end[LANGUAGE_NONE][0]['value'])) : print $speaker->field_media_event_end[LANGUAGE_NONE][0]['value']; endif; ?>"
                                    data-popcorn-title="<?php print $speaker->title ?>"
                                    data-popcorn-type="cue_field_popcornjs" class="cm-agenda-chapter-speaker"
                                ><?php print $speaker->title; ?></a>
                                <?php else : ?>
                                  <?php print $speaker->title; ?>
                                <?php endif; ?>
                              </li>
                            <?php endforeach; ?>
                          </ul>
                        <?php endif; ?>
                      <?php endif; ?>
                    </li>
                    <?php endforeach; ?>
                  </ul>
                </div>
              </div>
            </div>
          </div>
        <?php endif; ?>
        <?php
          $description_content = render($description);
          if (!empty($description_content) || user_access('administer agendas')) : ?>
            <div class="cm-agenda-tab cm-agenda-tab-description">
            <input class="cm-agenda-tab-radio" type="radio" id="cm-agenda-tab-info" name="cm-agenda-tab-group-tabs" <?php (empty($chapters)) ? print 'checked' : '' ?> >
            <?php if (empty($description_content)) : ?>
              <label class="cm-agenda-tab-label alert alert-warning" for="cm-agenda-tab-info"><?php print t('Description') ?></label>
            <?php else : ?>
              <label class="cm-agenda-tab-label" for="cm-agenda-tab-info"><?php print t('Description') ?></label>
            <?php endif; ?>

            <div class="cm-agenda-tab-panel">
              <div class="cm-agenda-tab-content">
                <?php if (!empty($description_content)) : ?>
                  <?php print $description_content; ?>
                <?php elseif (user_access('administer agendas')) : ?>
                  <?php print "<div class=\"alert alert-warning\">" . t('There is no description available for this agenda. This tab will be hidden for anonymous users.') . "</div" ?>
                <?php endif; ?>
              </div>
            </div>
          </div>
          <?php endif; ?>
      </div>
    <?php elseif (!empty($chapter_form)) : ?>
        <?php print render($chapter_form); ?>
      <?php endif; ?>
    <?php if(!empty($form)) : ?>
      <div class="cm-agenda-form"><?php print drupal_render_children($form); ?></div>
    <?php endif; ?>
  </div>
</article>
