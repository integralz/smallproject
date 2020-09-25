import requests
from collections import deque
url = 'http://localhost:8000'


def start(user, problem, count):
    uri = url + '/start' + '/' + user + '/' + str(problem) + '/' + str(count)
    return requests.post(uri).json()


def oncalls(token):
    uri = url + '/oncalls'
    return requests.get(uri, headers={'X-Auth-Token': token}).json()


def action(token, cmds):
    uri = url + '/action'
    return requests.post(uri, headers={'X-Auth-Token': token}, json={'commands': cmds}).json()

user = "test"
problem = 2
count = 4
building = 26
ans = 0
ans1 = 0
wait = [deque([]) for i in range(building)]
take = deque([])
elevator = [0 for _ in range(4)]
status = [[] for _ in range(4)]
pick = [deque([]) for _ in range(4)]
mat = [0 for _ in range(501)]
for i in range(4):
    status[i].append(1)
    status[i].append(1)
    status[i].append("stopped")

token = start(user, problem, count)
print(token)
token = token.get("token")

def calling():
    call = oncalls(token)
    call = call.get("calls")
    for i in range(len(call)):
        name = call[i].get("id")
        st = call[i].get("start")
        en = call[i].get("end")
        if mat[name] == 0:
            mat[name] = 1
            wait[st].append((name, en))
            take.append((name, st))


def che1(wh):
    for i in range(len(pick[wh])):
        if pick[wh][i][1] == status[wh][0]:
            return 1
    return 0

def che2(wh):
    if len(pick[wh]) < 8 and wait[status[wh][0]]:
        for i in range(len(wait[status[wh][0]])):
            if wait[status[wh][0]][i][1] > status[wh][0]:
                if status[wh][1] - status[wh][0] >= 0:
                    return 1
            if wait[status[wh][0]][i][1] < status[wh][0]:
                if status[wh][1] - status[wh][0] <= 0:
                    return 1
    return 0

while 1:
    calling()
    for i in range(4):
        if elevator[i] == 0 and take:
            elevator[i] = 1
            status[i][1] = take[0][1]
            take.popleft()
    cmds = []
    for i in range(4):
        if elevator[i] == 1:
            que = 0
            if status[i][2] == 'stopped':
                a = che1(i)
                b = che2(i)
                if a == 1:
                    cmds.append({
                        'elevator_id': i,
                        "command": "OPEN"
                    })
                    status[i][2] = 'opened'
                elif b == 1:
                    cmds.append({
                        'elevator_id': i,
                        "command": "OPEN"
                    })
                    status[i][2] = 'opened'
                else:

                    if status[i][1] - status[i][0] > 0:
                        cmds.append({
                            'elevator_id': i,
                            "command": "UP"
                        })
                        status[i][0] += 1
                        status[i][2] = 'upward'
                    elif status[i][1] - status[i][0] == 0:
                        if pick[i]:
                            status[i][1] = pick[i][0][1]

                            if status[i][1] < status[i][0]:
                                cmds.append({
                                    'elevator_id': i,
                                    "command": "DOWN"
                                })
                                status[i][0] -= 1
                                status[i][2] = 'downward'
                            else:
                                cmds.append({
                                    'elevator_id': i,
                                    "command": "UP"
                                })
                                status[i][0] += 1
                                status[i][2] = 'upward'
                        else:
                            elevator[i] = 0
                            cmds.append({
                                'elevator_id': i,
                                "command": "STOP"
                            })
                            status[i][2] = 'stopped'
                    else:
                        cmds.append({
                            'elevator_id': i,
                            "command": "DOWN"
                        })
                        status[i][0] -= 1
                        status[i][2] = 'downward'
            elif status[i][2] == 'opened':
                a = che1(i)
                b = che2(i)
                if a == 1:
                    sta = []
                    que = 0
                    for j in range(len(pick[i])):
                        if pick[i][que][1] == status[i][0]:
                            for k in range(len(take)):
                                if take[k][0] == pick[i][que][0]:
                                    del(take[k])
                                    break
                            sta.append(pick[i][que][0])
                            del(pick[i][que])
                            que -= 1
                        que += 1
                    cmds.append({
                        'elevator_id': i,
                        "command": "EXIT",
                        "call_ids" : sta
                    })
                    ans += len(sta)
                    status[i][2] = 'opened'
                elif b == 1:
                    sta = []
                    que = 0
                    wh1 = status[i][0]
                    for j in range(len(wait[wh1])):
                        if len(pick[i]) < 8:
                            if wait[wh1][que][1] > status[i][0]:
                                if status[i][1] - status[i][0] >= 0:
                                    pick[i].append(wait[wh1][que])
                                    sta.append(wait[wh1][que][0])
                                    del(wait[wh1][que])
                                    que -= 1
                            elif wait[wh1][que][1] < status[i][0]:
                                if status[i][1] - status[i][0] <= 0:
                                    pick[i].append(wait[wh1][que])
                                    sta.append(wait[wh1][que][0])
                                    del(wait[wh1][que])
                                    que -= 1
                        que += 1
                    cmds.append({
                        'elevator_id': i,
                        "command": "ENTER",
                        "call_ids": sta
                    })
                    print(sta)
                    status[i][2] = 'opened'
                    ans1 += len(sta)
                else:
                    cmds.append({
                        'elevator_id': i,
                        "command": "CLOSE"
                    })
                    status[i][2] = 'stopped'
            elif status[i][2] == 'upward':
                a = che1(i)
                b = che2(i)
                if a == 1:
                    cmds.append({
                        'elevator_id': i,
                        "command": "STOP"
                    })
                    status[i][2] = 'stopped'
                elif b == 1:
                    cmds.append({
                        'elevator_id': i,
                        "command": "STOP"
                    })
                    status[i][2] = 'stopped'
                else:
                    if status[i][1] - status[i][0] == 0:
                        if pick[i]:
                            status[i][1] = pick[i][0][1]
                            cmds.append({
                                'elevator_id': i,
                                "command": "UP"
                            })
                            status[i][0] += 1
                            status[i][2] = 'upward'
                        else:
                            elevator[i] = 0
                            cmds.append({
                                'elevator_id': i,
                                "command": "STOP"
                            })
                            status[i][2] = 'stopped'
                    else :
                        cmds.append({
                            'elevator_id': i,
                            "command": "UP"
                        })
                        status[i][0] += 1
                        status[i][2] = 'upward'
            else:
                a = che1(i)
                b = che2(i)
                if a == 1:
                    cmds.append({
                        'elevator_id': i,
                        "command": "STOP"
                    })
                    status[i][2] = 'stopped'
                elif b == 1:
                    cmds.append({
                        'elevator_id': i,
                        "command": "STOP"
                    })
                    status[i][2] = 'stopped'
                else:
                    if status[i][1] - status[i][0] == 0:
                        if pick[i]:
                            status[i][1] = pick[i][0][1]
                            cmds.append({
                                'elevator_id': i,
                                "command": "DOWN"
                            })
                            status[i][0] -= 1
                            status[i][2] = 'downward'
                        else:
                            elevator[i] = 0
                            cmds.append({
                                'elevator_id': i,
                                "command": "STOP"
                            })
                            status[i][2] = 'stopped'
                    else:
                        cmds.append({
                            'elevator_id': i,
                            "command": "DOWN"
                        })
                        status[i][0] -= 1
                        status[i][2] = 'downward'
        elif elevator[i] == 0:
            cmds.append({
                'elevator_id': i,
                "command": "STOP"
            })
            status[i][2] = 'stopped'
    action(token, cmds)
    if ans == 500:
        print(ans1)
        break

