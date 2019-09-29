def calculate(data, findall):
    matches = findall(
        r"([abc])([-+])?=([-+])?([abc]|\d+)(([+-])([abc]|\d+))?")
    for val1, sign_eq, sign_of_lhs, lhs, all_rhs, sign_of_rhs, rhs in matches:
        lhs = data[lhs] if lhs in data else int(lhs or 0)
        rhs = data[rhs] if rhs in data else int(rhs or 0)

        if sign_of_rhs == '-':
            rhs *= -1

        if sign_of_lhs == '-':
            lhs *= -1

        total_left = lhs + rhs
        if not sign_eq:
            data[val1] = total_left
        else:
            if sign_eq == '-':
                total_left *= -1

            data[val1] += total_left


    return data
