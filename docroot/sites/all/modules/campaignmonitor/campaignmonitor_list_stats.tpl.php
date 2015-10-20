<div class="campaignmonitor-list-stats">
  <h1><?php print $list['name'] ?></h1>

  <table>
    <thead>
      <td class="active"><?php print t('Subscribers'); ?></td>
      <td class="active"></td>
    </thead>
    <tbody>
      <tr class="odd">
        <td><?php print t('Total'); ?></td>
        <td><?php print $stats['TotalActiveSubscribers']; ?></td>
      </tr>
      <tr class="even">
        <td><?php print t('New today'); ?></td>
        <td><?php print $stats['NewActiveSubscribersToday']; ?></td>
      </tr>
      <tr class="odd">
        <td><?php print t('New yesterday'); ?></td>
        <td><?php print $stats['NewActiveSubscribersYesterday']; ?></td>
      </tr>
      <tr class="even">
        <td><?php print t('New this week'); ?></td>
        <td><?php print $stats['NewActiveSubscribersThisWeek']; ?></td>
      </tr>
      <tr class="odd">
        <td><?php print t('New this month'); ?></td>
        <td><?php print $stats['NewActiveSubscribersThisMonth']; ?></td>
      </tr>
      <tr class="even">
        <td><?php print t('New this year'); ?></td>
        <td><?php print $stats['NewActiveSubscribersThisYear']; ?></td>
      </tr>
    </tbody>
  </table>

  <table>
    <thead>
      <td class="active"><?php print t('Unsubscribers'); ?></td>
      <td class="active"></td>
    </thead>
    <tbody>
      <tr class="odd">
        <td><?php print t('Total'); ?></td>
        <td><?php print $stats['TotalUnsubscribes']; ?></td>
      </tr>
      <tr class="even">
        <td><?php print t('Today'); ?></td>
        <td><?php print $stats['UnsubscribesToday']; ?></td>
      </tr>
      <tr class="odd">
        <td><?php print t('Yesterday'); ?></td>
        <td><?php print $stats['UnsubscribesYesterday']; ?></td>
      </tr>
      <tr class="even">
        <td><?php print t('This week'); ?></td>
        <td><?php print $stats['UnsubscribesThisWeek']; ?></td>
      </tr>
      <tr class="odd">
        <td><?php print t('This month'); ?></td>
        <td><?php print $stats['UnsubscribesThisMonth']; ?></td>
      </tr>
      <tr class="even">
        <td><?php print t('This year'); ?></td>
        <td><?php print $stats['UnsubscribesThisYear']; ?></td>
      </tr>
    </tbody>
  </table>

  <table>
    <thead>
      <td class="active"><?php print t('Deleted'); ?></td>
      <td class="active"></td>
    </thead>
    <tbody>
      <tr class="odd">
        <td><?php print t('Total'); ?></td>
        <td><?php print $stats['TotalDeleted']; ?></td>
      </tr>
      <tr class="even">
        <td><?php print t('Today'); ?></td>
        <td><?php print $stats['DeletedToday']; ?></td>
      </tr>
      <tr class="odd">
        <td><?php print t('Yesterday'); ?></td>
        <td><?php print $stats['DeletedYesterday']; ?></td>
      </tr>
      <tr class="even">
        <td><?php print t('This week'); ?></td>
        <td><?php print $stats['DeletedThisWeek']; ?></td>
      </tr>
      <tr class="odd">
        <td><?php print t('This month'); ?></td>
        <td><?php print $stats['DeletedThisMonth']; ?></td>
      </tr>
      <tr class="even">
        <td><?php print t('This year'); ?></td>
        <td><?php print $stats['DeletedThisYear']; ?></td>
      </tr>
    </tbody>
  </table>

    <table>
    <thead>
      <td class="active"><?php print t('Bounces'); ?></td>
      <td class="active"></td>
    </thead>
    <tbody>
      <tr class="odd">
        <td><?php print t('Total'); ?></td>
        <td><?php print $stats['TotalBounces']; ?></td>
      </tr>
      <tr class="even">
        <td><?php print t('Today'); ?></td>
        <td><?php print $stats['BouncesToday']; ?></td>
      </tr>
      <tr class="odd">
        <td><?php print t('Yesterday'); ?></td>
        <td><?php print $stats['BouncesYesterday']; ?></td>
      </tr>
      <tr class="even">
        <td><?php print t('This week'); ?></td>
        <td><?php print $stats['BouncesThisWeek']; ?></td>
      </tr>
      <tr class="odd">
        <td><?php print t('This month'); ?></td>
        <td><?php print $stats['BouncesThisMonth']; ?></td>
      </tr>
      <tr class="even">
        <td><?php print t('This year'); ?></td>
        <td><?php print $stats['BouncesThisYear']; ?></td>
      </tr>
    </tbody>
  </table>
</div>
