const {spawnSync}=require('child_process');
const fs=require('fs'), os=require('os'), path=require('path');
const dir=fs.mkdtempSync(path.join(os.tmpdir(),'forca-teste-'));
const exe=path.resolve(process.argv[2]);
function run(input){const r=spawnSync(exe,[],{cwd:dir,input,encoding:'utf8',timeout:5000});if(r.status!==0)throw Error('Falha: '+r.stderr);return r.stdout}
try {
  let r=run('Ana\nBia\n1\n2\nanimais\nmico leão dourado\nm\ni\nc\no\nl\ne\na\nd\nu\nr\n1\n');
  if(!r.includes('Ana venceu a rodada') || !r.includes('Ana 1 x 0 Bia') || !r.includes('ã'))throw Error('Falhou partida acentuada');
  r=run('Ana\nBia\n1\n2\nanimais\ngato\nx\ny\nz\nw\n1\n');
  if(!r.includes('Ana 1 x 0 Bia') || !r.includes('Ana 1 x 1 Bia'))throw Error('Falhou carregamento do placar');
  r=run('Ana\nBia\n1\n2\nanimais\ngato\ng\na\nt\no\n1\n');
  if(!r.includes('Ana 2 x 1 Bia'))throw Error('Falhou salvar novamente');
  r=run('Ana\nBia\n1\n9\n3\ntema\ngato\nx\ny\nz\n1\n');
  if(!r.includes('Digite 1, 2 ou 3') || !r.includes('Erros restantes: 0') || !r.includes('Bia venceu!'))throw Error('Falhou dificil');
  r=run('Ana\nBia\n1\n1\ntema\ngato\nx\ny\nz\nw\nv\nu\n1\n');
  if(!r.includes('Erros restantes: 6') || !r.includes('Erros restantes: 0') || !r.includes('Bia venceu!'))throw Error('Falhou facil');
  console.log('Frase acentuada e placar entre tres execucoes verificados.');
} finally { const placar=path.join(dir,'placar-forca.txt'); if(fs.existsSync(placar))fs.unlinkSync(placar); fs.rmdirSync(dir); }

