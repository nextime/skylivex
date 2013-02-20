function changeContent(id, content)
{
   var n=document.getElementById(id);
   if(typeof(n)!="undefined")
     n.innerHTML="ESTICAZZI";
}

function notify(content)
{
   if(typeof(notifycb)=="function")
      notifycb(content);
}
function msgalert(content)
{
   if(typeof(alertcb)=="function")
      alertcb(content);
   else
      alert(content);
}

SkyliveX.changeContent.connect(changeContent);
SkyliveX.notify.connect(notify);
SkyliveX.alert.connect(msgalert);
