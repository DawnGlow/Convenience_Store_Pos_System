import random
from datetime import datetime, timedelta

# 아이템 ID 범위
item_ids = list(range(1, 11))

# 랜덤한 날짜 생성을 위한 시작 날짜
start_date = datetime(2022, 1, 1)

# 랜덤 데이터 생성 및 파일 작성
with open("random_purchase_data.txt", "w") as file:
    for _ in range(1000):
        # 영수증 날짜 생성
        purchase_date = start_date + timedelta(days=random.randint(0, 730))  # 약 2년간의 기간에서 랜덤 선택

        # 영수증 정보 작성
        file.write(f"{purchase_date.year} {purchase_date.month:02d} {purchase_date.day:02d} "
                   f"{random.randint(0, 23):02d} {random.randint(0, 59):02d} {random.randint(10000, 99999)}\n")

        # 아이템 판매 정보 작성
        num_items = random.randint(1, 10)  # 1개에서 10개 사이의 랜덤한 수량 선택
        for _ in range(num_items):
            item_id = random.choice(item_ids)
            quantity = random.randint(1, 10)  # 1개에서 10개 사이의 랜덤한 수량 선택
            file.write(f"{item_id} {quantity}\n")

        # 영수증 사이에 개행 추가
        file.write("\n")