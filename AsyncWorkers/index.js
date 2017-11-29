// require the module from this package.json
var m = require('./');

// adjust execution time by iterations
var iterations = 1000000000;
var logMsg = 'execution time:';

function runSync(numTasks) {
  console.time(logMsg);
  for (let i = 0; i < numTasks; i++) {
    var taskId = m.doSyncStuff('syncTask_' + i, iterations);
    console.log('task %s finished', taskId);
  }
  console.timeEnd(logMsg);
}

// wrap the callbacked doAsyncStuff in a promise
function doAsyncStuffPromised(workerId, throwsError = false) {
  return new Promise(function(resolve, reject) {

    // this is how we would invoke the function with a callback
    m.doAsyncStuff(workerId, iterations, throwsError, function(error, result) {
      if (error) {
        return reject(error);
      }
      return resolve(result);
    });

  });
}

function runAsync(numTasks) {
  var taskIds = Array(numTasks).fill(0).map((_, i) => 'asyncTask_' + i);
  var promises = taskIds.map(
    taskId => doAsyncStuffPromised(taskId)
      .then((tId) => {
        console.log('task %s finished', tId);
        return tId;
      })
  );

  console.time(logMsg);
  return Promise.all(promises)
    .then(taskIds => {
      console.log('all tasks finished:', taskIds);
      console.timeEnd(logMsg);
    })
    .catch(error => console.log(error))
}

runSync(4);
runAsync(4).then(() => {
  // after running the async example lets see what happens
  // if we trigger an error
  console.log('Expecting an error message soon:')
  doAsyncStuffPromised('foo', true)
    .then(taskIds => console.timeEnd(logMsg))
    .catch(error => console.log(error));
});


