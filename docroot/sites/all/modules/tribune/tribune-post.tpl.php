<li class='tribune-post <?php echo $zebra; ?>'
    value='<?php echo $post['id'] ?>'
    id='tribune-<?php echo $post['id'] ?>'
    data-user='<?php echo $post['uid'] ?>'
    data-username='<?php echo htmlspecialchars($post['username']) ?>'
    data-timestamp='<?php echo $post['timestamp'] ?>'
    data-clockindex='<?php echo $post['clock_index'] ?>'
    data-tribune='<?php echo $post['tribune']->nid ?>'
    data-postid='<?php echo $post['id'] ?>'
><span class='tribune-date'><?php echo date('Y-m-d', $post['unixtimestamp']) ?></span><?php echo $post['prefix'] ?>
    <span class='tribune-clock' title=''><?php echo $post['clock'] ?></span>
    <span class='tribune-user' title='<?php echo $post['info'] ?>'><?php echo ($post['username'] ?: ($post['mini-info'] ?: htmlspecialchars($post['info']))) ?></span>
    <span class='tribune-message'><?php echo $post['text'] ?></span><?php echo $post['suffix'] ?></li>
