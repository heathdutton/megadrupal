<table <?php if ($classes) { print 'class="'. $classes . '" '; } ?><?php print $attributes; ?>>
  <tbody>
    <?php foreach ($job_board as $company_name => $row): ?>
      <tr class="<?php print $row['sticky'] ? 'sticky' : '' ?>">
        <?php if (isset($row['logo'])): ?>
          <td class="company-logo"><?php print $row['logo'] ?></td>
        <?php endif ?>

        <?php if (isset($row['company_name'])): ?>
          <td class="company-name"><?php print $row['company_name'] ?></td>
        <?php endif ?>

        <?php if (isset($row['jobs'])): ?>
          <td class="company-jobs">
            <?php foreach ($row['jobs'] as $type => $jobs): ?>
              <?php print theme('item_list', array('items' => $jobs)) ?>
            <?php endforeach ?>
          </td>
        <?php endif ?>

      </tr>
    <?php endforeach; ?>
  </tbody>
</table>