window.onload = function ShallowGrungeChangeColor(){
  headcolor=document.getElementById('edit-headcolor');
  headcolor.type='color';
  
  navcolor=document.getElementById('edit-navcolor');
  navcolor.type='color';

  headingscolor=document.getElementById('edit-headingscolor');
  headingscolor.type='color';

  linkcolor=document.getElementById('edit-linkcolor');
  linkcolor.type='color';

}

function ShallowGrungeResetColors(){
  headcolor=document.getElementById('edit-headcolor');
  headcolor.value = '#333333';
  
  navcolor=document.getElementById('edit-navcolor');
  navcolor.value='#7f0000';

  headingscolor=document.getElementById('edit-headingscolor');
  headingscolor.value='#000000';

  linkcolor=document.getElementById('edit-linkcolor');
  linkcolor.value='#A42424';
}
