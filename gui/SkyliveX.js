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

SkyliveX.changeContent.connect(changeContent);
SkyliveX.notify.connect(notify);
