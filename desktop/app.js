'use strict';
(() => {
  const secure = location.protocol === 'https:';
  document.querySelector('#transport').textContent = secure ? 'HTTPS · ejecución local' : 'HTTPS requerido';
  if (!secure) {
    document.querySelector('#blocked').hidden = false;
    document.querySelectorAll('nav button').forEach(b => { b.disabled = true; });
    return;
  }
  const host = document.querySelector('#windows');
  const open = new Map(); let z = 2;
  function windowFor(key, title, create) {
    if (open.has(key)) { open.get(key).style.zIndex = ++z; return; }
    const pane = document.createElement('section'); pane.className = 'window';
    pane.style.left = `${24 + open.size * 38}px`; pane.style.top = `${open.size * 22}px`; pane.style.zIndex = ++z;
    const bar = document.createElement('div'); bar.className = 'titlebar';
    const heading = document.createElement('h2'); heading.textContent = title;
    const close = document.createElement('button'); close.textContent = '×'; close.setAttribute('aria-label',`Cerrar ${title}`);
    bar.append(heading, close); const content = document.createElement('div'); content.className = 'content';
    pane.append(bar, content); host.append(pane); open.set(key, pane);
    const dispose = create(content) || (() => {});
    close.addEventListener('click', () => { dispose(); pane.remove(); open.delete(key); });
    pane.addEventListener('pointerdown', () => { pane.style.zIndex = ++z; });
    bar.addEventListener('pointerdown', e => {
      if (e.target === close) return;
      const startX=e.clientX, startY=e.clientY, left=pane.offsetLeft, top=pane.offsetTop;
      bar.setPointerCapture(e.pointerId);
      const move = m => { pane.style.left=`${Math.max(0,left+m.clientX-startX)}px`; pane.style.top=`${Math.max(0,top+m.clientY-startY)}px`; };
      const end = () => { bar.removeEventListener('pointermove',move); bar.removeEventListener('pointerup',end); bar.removeEventListener('pointercancel',end); };
      bar.addEventListener('pointermove',move); bar.addEventListener('pointerup',end); bar.addEventListener('pointercancel',end);
    });
  }
  const apps = {
    notes: () => windowFor('notes','Notas en este navegador',c => {
      const hint=document.createElement('p'); hint.className='muted'; hint.textContent='Estas notas se guardan en tu navegador, sin enviarlas al router.';
      const input=document.createElement('textarea'); input.setAttribute('aria-label','Notas');
      try { input.value=localStorage.getItem('hgw-desktop-notes') || ''; } catch { hint.textContent='El navegador no permite almacenamiento local; las notas duran esta sesión.'; }
      input.addEventListener('input',()=> { try { localStorage.setItem('hgw-desktop-notes',input.value); } catch { hint.textContent='No se pudieron guardar las notas. Conserva una copia antes de cerrar.'; } });
      c.append(hint,input); input.focus();
    }),
    about: () => windowFor('about','Cómo funciona',c => {
      for (const text of ['El HGW atiende la petición HTTPS y entrega archivos estáticos.','Snake, las ventanas y las notas se ejecutan en tu navegador. No hay un escritorio gráfico nativo ni un Docker corriendo dentro del router.','El port del driver sigue en desarrollo. Esta pantalla no mide ni certifica el estado físico de las radios.','Doom y futuros emuladores se añadirán con recursos locales y pruebas HTTPS propias.']) { const p=document.createElement('p'); p.textContent=text; c.append(p); }
    }),
    snake: () => windowFor('snake','Snake · cliente',c => {
      const canvas=document.createElement('canvas'); canvas.width=320; canvas.height=320; canvas.tabIndex=0; canvas.setAttribute('aria-label','Snake, usa las flechas o los botones');
      const status=document.createElement('p'); status.className='muted'; const controls=document.createElement('div'); controls.className='game-controls';
      c.append(canvas,status,controls); const ctx=canvas.getContext('2d'); let body,dir,next,food,score,alive,timer;
      const emptyFood=()=> { const cells=[]; for(let y=0;y<20;y++)for(let x=0;x<20;x++)if(!body.some(p=>p.x===x&&p.y===y))cells.push({x,y}); return cells[Math.floor(Math.random()*cells.length)]; };
      const draw=()=> { ctx.fillStyle='#081320';ctx.fillRect(0,0,320,320);ctx.fillStyle='#89cc9c';body.forEach(p=>ctx.fillRect(p.x*16+1,p.y*16+1,14,14));if(food){ctx.fillStyle='#e4ac75';ctx.fillRect(food.x*16+2,food.y*16+2,12,12);}status.textContent=alive?`Puntos: ${score} · flechas para moverte`:`Puntos: ${score} · ${body.length===400?'Has ganado':'Fin de partida'}`; };
      const tick=()=> { if(!alive)return;dir=next;const head={x:body[0].x+dir.x,y:body[0].y+dir.y};const eats=food&&head.x===food.x&&head.y===food.y;const occupied=eats?body:body.slice(0,-1);if(head.x<0||head.x>=20||head.y<0||head.y>=20||occupied.some(p=>p.x===head.x&&p.y===head.y)){alive=false;clearInterval(timer);draw();return;}body.unshift(head);if(eats){score++;food=emptyFood();if(!food){alive=false;clearInterval(timer);}}else body.pop();draw(); };
      const start=()=> {clearInterval(timer);body=[{x:7,y:10},{x:6,y:10},{x:5,y:10}];dir={x:1,y:0};next=dir;score=0;alive=true;food=emptyFood();draw();timer=setInterval(tick,140);canvas.focus();};
      const directions={ArrowUp:{x:0,y:-1},ArrowDown:{x:0,y:1},ArrowLeft:{x:-1,y:0},ArrowRight:{x:1,y:0}};
      const turn=k=> {const d=directions[k];if(d&&(d.x!==-dir.x||d.y!==-dir.y))next=d;};
      canvas.addEventListener('keydown',e=> {if(directions[e.key]){e.preventDefault();turn(e.key);}});
      for(const [key,label] of [['ArrowUp','↑'],['ArrowLeft','←'],['ArrowDown','↓'],['ArrowRight','→'],['Restart','Reiniciar']]){const b=document.createElement('button');b.textContent=label;b.setAttribute('aria-label',key==='Restart'?'Reiniciar':`Mover ${label}`);b.addEventListener('click',()=>key==='Restart'?start():turn(key));controls.append(b);}
      const visibility=()=> {if(document.hidden){clearInterval(timer);}else if(alive){clearInterval(timer);timer=setInterval(tick,140);}};
      document.addEventListener('visibilitychange',visibility);start();return()=> {clearInterval(timer);document.removeEventListener('visibilitychange',visibility);};
    })
  };
  document.querySelectorAll('[data-open]').forEach(b=>b.addEventListener('click',()=>apps[b.dataset.open]()));
  const clock=document.querySelector('#clock');const refresh=()=> {clock.textContent=new Date().toLocaleTimeString('es-ES',{hour:'2-digit',minute:'2-digit'});};refresh();setInterval(refresh,30000);
})();
