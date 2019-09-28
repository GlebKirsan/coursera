import requests
from collections import defaultdict


def calc_age(uid):
    TOKEN = '17da724517da724517da72458517b8abce117da17da72454d235c274f1a2be5f45ee711'
    user_request_url = 'https://api.vk.com/method/users.get'
    frineds_request_url = 'https://api.vk.com/method/friends.get'

    user_info = requests.get(user_request_url,
                             params={
                                 'access_token': TOKEN,
                                 'user_ids': uid,
                                 'v': 5.71
                             })

    json_user_info = user_info.json()
    integer_user_id = json_user_info['response'][0]['id']
    user_friends = requests.get(frineds_request_url,
                                params={
                                    'access_token': TOKEN,
                                    'user_id': integer_user_id,
                                    'v': 5.71,
                                    'fields': 'bdate'
                                })

    user_friends_response = user_friends.json()['response']
    user_friends = user_friends_response['items']
    age_count = defaultdict(int)
    current_year = 2019
    for user in user_friends:
        if 'bdate' not in user:
            continue

        day, month, *year = user['bdate'].split('.')

        if not year:
            continue

        year = int(year[0])
        age_count[current_year - year] += 1

    tuples = list(age_count.items())
    return sorted(tuples, key=lambda x: (x[1], -x[0]), reverse=True)


if __name__ == '__main__':
    res = calc_age('reigning')
    print(res)
