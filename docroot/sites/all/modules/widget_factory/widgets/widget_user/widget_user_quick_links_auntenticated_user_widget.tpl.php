<ul class="nav navbar-top-links navbar-right">
  <li class="dropdown">
      <a class="dropdown-toggle" data-toggle="dropdown" href="#" aria-expanded="false">
          <i class="fa fa-user fa-fw"></i>  <i class="fa fa-caret-down"></i>
      </a>
      <ul class="dropdown-menu dropdown-user">
          <li><a href="/user"><i class="fa fa-user fa-fw"></i> User Profile</a>
          </li>
          <li><a href="/user/<?php echo $account->uid; ?>/edit"><i class="fa fa-gear fa-fw"></i> Settings</a>
          </li>
          <li class="divider"></li>
          <li><a href="/user/logout"><i class="fa fa-sign-out fa-fw"></i> Logout</a>
          </li>
      </ul>
      <!-- /.dropdown-user -->
  </li>
</ul>
