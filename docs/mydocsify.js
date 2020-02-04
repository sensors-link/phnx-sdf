// 引入docsify-themeable主题
// document.write('<link rel="stylesheet" href="https://cdn.bootcss.com/docsify/4.9.4/themes/vue.css" />')
document.write('<link rel="stylesheet" href="https://unpkg.com/docsify/lib/themes/vue.css" />')
// 用polyfill兼容IE
// document.write('<script src="https://cdn.bootcss.com/babel-polyfill/7.6.0/polyfill.min.js"></script>')
document.write('<script src="https://unpkg.com/babel-polyfill/dist/polyfill.min.js"></script>')
// 加载docsify
// document.write('<script src="https://cdn.bootcss.com/docsify/4.9.4/docsify.min.js"></script>')
document.write('<script src="https://unpkg.com/docsify/lib/docsify.min.js"></script>')
// Docsify 全文搜索插件
// document.write('<script src="https://cdn.bootcss.com/docsify/4.9.4/plugins/search.min.js"></script>')
document.write('<script src="https://unpkg.com/docsify/lib/plugins/search.min.js"></script>')
// Docsify ZoomImage插件
// document.write('<script src="https://cdn.bootcss.com/docsify/4.9.4/plugins/zoom-image.min.js"></script>')
document.write('<script src="https://unpkg.com/docsify/lib/plugins/zoom-image.min.js"></script>')
// Plantuml 插件
document.write('<script src="https://unpkg.com/docsify-plantuml/dist/docsify-plantuml.min.js"></script>')
//  collapse sidebar插件
// document.write('<script src="//unpkg.com/docsify-sidebar-collapse/dist/docsify-sidebar-collapse.min.js">')
// x.html#/xxx 形式的链接， Docsify会认为x.html是上级目录，哎！补丁一下
var basePath = location.pathname.endsWith('.html') ? '..' : '.'

window.$docsify = {
  basePath: basePath,
  name: document.title,
  loadSidebar: '_sidebar.md',
  maxLevel: 4,
  subMaxLevel: 4,
  auto2top: true,
  loadNavbar: false,
  // loadNavbar: '_navbar.md',
  executeScript: true,
  search: {
    paths: 'auto',
    placeholder: '全文搜索',
    noData: '未找到结果'
  },
  plantuml: {
    skin: 'classic'
  }
}