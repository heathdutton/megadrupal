<?php

/**
 * @file contest-admin.tpl.php
 * Template for a contest's admin page.
 *
 * Available variables:
 * - $data (object)
 * - - node (object) contest node object
 * - - contest (object)
 * - - - start (int)
 * - - - end (int)
 * - - - places (int)
 * - - - period (int)
 * - - - publish_winners (bool)
 * - - - entrants (int)
 * - - - entries (int)
 * - - contestants (array of objects) uid => contestant object
 * - - - uid (int)
 * - - - name (string)
 * - - - mail (string)
 * - - - qty (int)
 * - - - created (int)
 * - - - winner (bool)
 * - - host (object)
 * - - - uid (int)
 * - - - name (string)
 * - - - mail (string)
 * - - - title (string)
 * - - - full_name (string)
 * - - - business (string)
 * - - - address (string)
 * - - - city (string)
 * - - - state (string)
 * - - - zip (string)
 * - - - phone (string)
 * - - - birthdate (int)
 * - - sponsor (object)
 * - - - uid (int)
 * - - - name (string)
 * - - - mail (string)
 * - - - url (string)
 * - - - full_name (string)
 * - - - business (string)
 * - - - address (string)
 * - - - city (string)
 * - - - state (string)
 * - - - zip (string)
 * - - - phone (string)
 * - - - birthdate (int)
 * - - winners (array) uid => place
 */
?>
<div id="contest-admin">

<!-- Some host details. --->

  <fieldset class="contest-admin-host">
    <legend><?php print t('Host'); ?></legend>
    <?php print l($data->host->title, "user/{$data->host->uid}"); ?><br />
    <?php print l($data->host->mail, "mailto:{$data->host->mail}", array('absolute' => TRUE)); ?><br />
    <?php print t('Phone: @phone', array('@phone' => $data->host->phone)); ?><br />
    <?php print t('Address: @address', array('@address' => "{$data->host->address}, {$data->host->city} {$data->host->state} {$data->host->zip}")); ?>
  </fieldset>

<!-- Some sponsor details. --->

  <fieldset class="contest-admin-sponsor">
    <legend><?php print t('Sponsor'); ?></legend>
    <?php print l($data->contest->sponsor->name, "user/{$data->contest->sponsor->uid}"); ?><br />
    <?php print l($data->contest->sponsor->mail, "mailto:{$data->contest->sponsor->mail}", array('absolute' => TRUE)); ?><br />
    <?php print l(preg_replace('/https?:\/\//', '', $data->contest->sponsor->url), $data->contest->sponsor->url, array('absolute' => TRUE)); ?>
  </fieldset>

<!-- Some contest details. --->

  <div class="contest-admin-detail">
    <?php print t('Start Date: !start_date', array('!start_date' => format_date($data->contest->start, 'long'))); ?><br />
    <?php print t('End Date: !end_date', array('!end_date' => format_date($data->contest->end, 'long'))); ?><br />
    <?php print t('Total Entries: !entries', array('!entries' => $data->contest->entries)); ?><br />
    <?php print t('Total Users: !entrants', array('!entrants' => $data->contest->entrants)); ?><br />
    <?php print t('Places Allowed: !places', array('!places' => $data->contest->places)); ?>
  </div>


<!-- The administration actions. --->

<?php if ($data->contest->end < REQUEST_TIME): ?>
  <ul class="contest-admin-actions">

  <?php if (count($data->winners) < $data->contest->places): ?>
    <li><?php print l(t('Pick Random Winner'), "contest/pick-winner/{$data->node->nid}"); ?></li>
  <?php else: ?>
    <li class="inactive"><?php print t('Pick Random Winner'); ?></li>
  <?php endif; ?>

  <?php if (count($data->winners) == $data->contest->places): ?>
    <li><?php print $data->contest->publish_winners? l(t('Unpublish Winners'), "contest/unpublish-winners/{$data->node->nid}"):  l(t('Publish Winners'), "contest/publish-winners/{$data->node->nid}"); ?></li>
  <?php else: ?>
    <li class="inactive"><?php print t('Publish Winners'); ?></li>
  <?php endif; ?>

  <?php if (count($data->winners) && !$data->contest->publish_winners): ?>
    <li><?php print l(t('Clear All Winners'), "contest/clear-winners/{$data->node->nid}"); ?></li>
  <?php else: ?>
    <li class="inactive"><?php print t('Clear All Winners'); ?></li>
  <?php endif; ?>

    <li><?php print l(t('Export Entries'), "contest/export-entries/{$data->node->nid}"); ?></li>
    <li><?php print l(t('Export Unique Users'), "contest/export-unique/{$data->node->nid}"); ?></li>
  </ul>
<?php else: ?>
  <ul class="contest-admin-actions">
    <li class="inactive"><?php print t('Pick Random Winner'); ?></li>
    <li class="inactive"><?php print t('Publish Winners'); ?></li>
    <li class="inactive"><?php print t('Clear All Winners'); ?></li>
    <li class="inactive"><?php print t('Export Entries'); ?></li>
    <li class="inactive"><?php print t('Export Unique Users'); ?></li>
  </ul>
<?php endif; ?>


<!-- The contest winners. --->

<?php if (!empty($data->winners)): ?>
  <table border="0" cellspacing="0" class="contest-admin-winners">
    <caption><?php print t('Contest Winners'); ?></caption>
    <thead>
      <tr>
        <th><?php print t('Place'); ?></th>
        <th><?php print t('Name'); ?></th>
        <th><?php print t('Email'); ?></th>
        <th><?php print t('Operation'); ?></th>
      </tr>
    </thead>
    <tbody>

    <?php $index = 0; ?>
    <?php foreach ($data->winners as $uid => $place): ?>
      <?php if (empty($data->contestants[$uid])): ?>
        <?php continue; ?>
      <?php else: ?>
        <?php $usr = $data->contestants[$uid]; ?>
      <?php endif; ?>

      <?php $index++; ?>
      <tr class="<?php print ($index % 2)? 'odd': 'even'; ?>">
        <td><?php print $place; ?>.</td>
        <td><?php print l($usr->name, "user/$usr->uid"); ?></td>
        <td><?php print l($usr->mail, "mailto:$usr->mail", array('absolute' => TRUE)); ?></td>
        <td><?php print l(t('Clear'), "contest/clear-winners/{$data->node->nid}/$usr->uid"); ?></td>
      </tr>
    <?php endforeach; ?>

    </tbody>
  </table>
<?php endif; ?>


<!-- The contest contestants. --->

  <table border="0" cellspacing="0" class="contest-admin-contestants">
    <caption><?php print t('Contest Entrants'); ?></caption>
    <thead>
      <tr>
        <th><?php print t('Name'); ?></th>
        <th><?php print t('Email'); ?></th>
        <th><?php print t('Count'); ?></th>
        <th><?php print t('Operation'); ?></th>
      </tr>
    </thead>
    <tbody>

    <?php $index = 0; ?>
    <?php foreach ($data->contestants as $usr): ?>
      <?php $index++; ?>

      <?php if (($index % 50) === 0): ?>
      
<!-- Print the header every 50 rows. --->

      <tr>
        <th><?php print t('Name'); ?></th>
        <th><?php print t('Email'); ?></th>
        <th><?php print t('Count'); ?></th>
        <th><?php print t('Operation'); ?></th>
      </tr>
      <?php endif; ?>

      <tr class="<?php print ($index % 2)? 'odd': 'even'; ?><?php print $usr->winner? ' winner': ''; ?>">
        <td><?php print l($usr->name, "user/$usr->uid"); ?></td>
        <td><?php print l($usr->mail, "mailto:$usr->mail", array('absolute' => TRUE)); ?></td>
        <td><?php print $usr->qty; ?></td>

      <?php if ($data->contest->end < REQUEST_TIME): ?>
        <td><?php print $usr->winner? l(t('Clear'), "contest/clear-winners/{$data->node->nid}/$usr->uid"): l(t('Pick'), "contest/pick-winner/{$data->node->nid}/$usr->uid"); ?></td>
      <?php else: ?>
        <td>&mdash;</td>
      <?php endif; ?>

      </tr>
    <?php endforeach; ?>

    </tbody>
  </table>
</div>
