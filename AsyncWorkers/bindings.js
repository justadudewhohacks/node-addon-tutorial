var myModule

if (process.env.DEBUG) {
  myModule= require('./build/Debug/myModule.node')
} else {
  myModule= require('./build/Release/myModule.node')
}

module.exports = myModule